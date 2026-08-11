#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>

#include "shader.h"
#include "path.h"
#include "file.h"
#include "log.h"

static void _warn( const char *fmt, ... )
{
	va_list args;

	fprintf( stderr, "SHADER: " );

	va_start( args, fmt );
	vfprintf( stderr, fmt, args );
	va_end( args );
}

static int shader_cache_uniform( struct shader *s, const char *name, GLint loc )
{
	size_t len;

	assert( s != NULL );
	assert( name != NULL );

	if ( loc < 0 )
		return -EINVAL;

	if ( s->u_count >= SHADER_UNIFORM_CACHE_MAX )
		return -ENOMEM;

	len = strlen( name );
	if ( len >= SHADER_UNIFORM_NAME_MAX ) {
		_warn("UNIFORM NAME TOO LONG, NOT CACHED: %s\n", name);
		return -EINVAL;
	}

	snprintf( s->uniforms[s->u_count].name, SHADER_UNIFORM_NAME_MAX,
	          "%s", name );

	s->uniforms[s->u_count].loc = loc;
	s->u_count++;
	return 0;
}

static GLint shader_get_uniform_loc( struct shader *s, const char *name )
{
	GLint loc;
	GLint err;

	assert( s != NULL );
	assert( name != NULL );
	assert( s->id != 0 );

	for ( size_t i = 0; i < s->u_count; i++ ) {
		struct shader_uniform *u = &s->uniforms[i];

		if ( strcmp( u->name, name ) == 0 )
			return u->loc;
	}

	loc = glGetUniformLocation( s->id, name );

	err = (GLint)shader_cache_uniform( s, name, loc );
	if ( err < 0 )
		return err;

	return loc;
}

static GLuint shader_compile( GLenum type, const char *src )
{
	int success;
	GLuint id;

	assert( src != NULL );

	id = glCreateShader( type );

	if ( id == 0 ) {
		_warn("glCreateShader FAILED FOR TYPE %d\n", (int)type);
		return 0;
	}
	
	glShaderSource( id, 1, &src, NULL );
	glCompileShader( id );
	glGetShaderiv( id, GL_COMPILE_STATUS, &success );

	if ( !success ) {
		char info_log[512];
		glGetShaderInfoLog( id, 512, NULL, info_log );

		switch ( type ) {
		case GL_VERTEX_SHADER:
			_warn("VERTEX COMPILATION FAILED: %s\n", info_log);
			break;
		case GL_FRAGMENT_SHADER:
			_warn("FRAGMENT COMPILATION FAILED: %s\n", info_log);
			break;
		default:
			_warn( info_log );
			break;
		};

		glDeleteShader( id );
		return 0;
	}

	return id;
}

static int shader_link_program( GLuint vert, GLuint frag, GLuint *out )
{
	GLuint id;
	int success;

	assert( out != NULL );

	id = glCreateProgram();

	if ( id == 0 ) {
		_warn("UNABLE TO CREATE PROGRAM\n");
		return -ENOMEM;
	}

	glAttachShader( id, vert );
	glAttachShader( id, frag );
	glLinkProgram( id );
	glGetProgramiv( id, GL_LINK_STATUS, &success );

	glDetachShader( id, vert );
	glDetachShader( id, frag );

	if ( !success ) {
		char info_log[512];

		glGetProgramInfoLog( id, 512, NULL, info_log );
		_warn( info_log );

		glDeleteProgram( id );
		return -EINVAL;
	}

	*out = id;
	return 0;
}

static int shader_init_from_src( struct shader *s, const char *vert_src,
                                 const char *frag_src )
{
	GLuint vert, frag;
	int err;

	assert( s != NULL );
	assert( vert_src != NULL );
	assert( frag_src != NULL );

	vert = shader_compile( GL_VERTEX_SHADER, vert_src );
	frag = shader_compile( GL_FRAGMENT_SHADER, frag_src );

	if ( !vert || !frag ) {
		if ( vert )
			glDeleteShader( vert );
		if ( frag )
			glDeleteShader( frag );

		return -EINVAL;
	}

	err = shader_link_program( vert, frag, &s->id );

	glDeleteShader( vert );
	glDeleteShader( frag );

	if ( err )
		return err;

	s->u_count = 0;
	return 0;
}

int shader_init( struct shader *s, const char *vert_path, const char *frag_path )
{
	char *vert_src = NULL;
	char *frag_src = NULL;
	int err;

	assert( s != NULL );
	assert( vert_path != NULL );
	assert( frag_path != NULL );

	if ( !file_read_text_rel( vert_path, &vert_src ) )
		return -EINVAL;
	if ( !file_read_text_rel( frag_path, &frag_src ) ) {
		file_free( vert_src );
		return -EINVAL;
	}

	if ( !vert_src || !frag_src ) {
		_warn("FILE READ REPORTED SUCCESS WITH NULL BUFFER\n");

		file_free( vert_src );
		file_free( frag_src );

		return -EINVAL;
	}

	err = shader_init_from_src( s, vert_src, frag_src );

	file_free( vert_src );
	file_free( frag_src );

	return err;
}

int shader_init_preset( struct shader *s, enum shader_builtin preset )
{
	const char *vert_path;
	const char *frag_path;

	switch ( preset ) {
	case SHADER_DEFAULT:
		vert_path = "src/graphics/shader/glsl/default_vsh.glsl";
		frag_path = "src/graphics/shader/glsl/default_fsh.glsl";
		break;
	case SHADER_PRIMITIVE_2D:
		vert_path = "src/graphics/shader/glsl/primitive2d_vsh.glsl";
		frag_path = "src/graphics/shader/glsl/primitive2d_fsh.glsl";
		break;
	default:
		vert_path = "src/graphics/shader/glsl/default_vsh.glsl";
		frag_path = "src/graphics/shader/glsl/default_fsh.glsl";
		break;
	}

	return shader_init( s, vert_path, frag_path );
}

void shader_use( const struct shader *s )
{
	assert( s != NULL );
	assert( s->id != 0 );

	glUseProgram( s->id );
}

bool shader_set_1i( struct shader *s, const char *name, int value )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform1i( loc, value );
	
	return true;
}

bool shader_set_1f( struct shader *s, const char *name, float value )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform1f( loc, value );
	
	return true;
}

bool shader_set_2f( struct shader *s, const char *name, const float value[2] )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform2fv( loc, 1, value );
	
	return true;
}

bool shader_set_vec2( struct shader *s, const char *name, vec2 v )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform2f( loc, v.x, v.y );
	
	return true;
}

bool shader_set_3i( struct shader *s, const char *name, const int value[3] )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform3iv( loc, 1, value );
	
	return true;
}

bool shader_set_ivec3( struct shader *s, const char *name, ivec3 v )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform3i( loc, v.x, v.y, v.z );
	
	return true;
}

bool shader_set_3f( struct shader *s, const char *name, const float value[3] )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform3fv( loc, 1, value );
	
	return true;
}

bool shader_set_vec3( struct shader *s, const char *name, vec3 v )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform3f( loc, v.x, v.y, v.z );
	
	return true;
}

bool shader_set_4f( struct shader *s, const char *name, const float value[4] )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform4fv( loc, 1, value );
	
	return true;
}

bool shader_set_vec4( struct shader *s, const char *name, vec4 v )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniform4f( loc, v.x, v.y, v.z, v.w );
	
	return true;
}

bool shader_set_mat4( struct shader *s, const char *name, const mat4 mat )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return false;

	glUniformMatrix4fv( loc, 1, GL_FALSE, (const float *)mat );
	
	return true;
}

void shader_destroy( struct shader *s )
{
	if ( !s )
		return;

	glDeleteProgram( s->id );

	s->id = 0;
	s->u_count = 0;
}
