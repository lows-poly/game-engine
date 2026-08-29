#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "shader.h"
#include "file.h"
#include "log.h"

static int shader_cache_uniform( struct shader *s, const char *name, GLint loc )
{
	size_t len;

	if ( !s || !name )
		return -EINVAL;

	if ( s->u_count >= SHADER_UNIFORM_CACHE_MAX )
		return -ENOMEM;

	len = strlen( name );
	if ( len >= SHADER_UNIFORM_NAME_MAX ) {
		pr_warn("UNIFORM NAME TOO LONG, NOT CACHED: %s\n", name);
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

	if ( !s ) {
		pr_warn("FAILED TO GET UNIFORM LOC: NULL SHADER\n");
		return 0;
	}

	if ( !name ) {
		pr_warn("FAILED TO GET UNIFORM LOC: NULL UNIFORM NAME\n");
		return 0;
	}

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

	if ( !src ) {
		pr_warn("UNABLE TO FIND SHADER SOURCE\n");
		return 0;
	}

	id = glCreateShader( type );

	if ( id == 0 ) {
		pr_warn("FAILED TO CREATE SHADER FOR TYPE: %d\n", (int)type);
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
			pr_warn("VERTEX COMPILATION FAILED: %s\n", info_log);
			break;
		case GL_FRAGMENT_SHADER:
			pr_warn("FRAGMENT COMPILATION FAILED: %s\n", info_log);
			break;
		default:
			pr_warn( info_log );
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

	if ( !out )
		return -EINVAL;

	id = glCreateProgram();

	if ( id == 0 ) {
		pr_err("NO MEMORY FOR SHADER PROGRAM\n");
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
		pr_warn( info_log );

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

	if ( !s )
		return -EINVAL;

	if ( !vert_src || !frag_src ) {
		pr_warn("NO VERTEX/FRAGMENT SHADER SOURCES\n");
		return -EINVAL;
	}

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

	if ( !s )
		return -EINVAL;

	if ( !vert_path || !frag_path ) {
		pr_warn("UNABLE TO FIND SHADER PATHS\n");
		return -EINVAL;
	}

	if ( !file_read_text_rel( vert_path, &vert_src ) )
		return -EINVAL;
	if ( !file_read_text_rel( frag_path, &frag_src ) ) {
		file_free( vert_src );
		return -EINVAL;
	}

	if ( !vert_src || !frag_src ) {
		pr_warn("FILE READ REPORTED SUCCESS WITH NULL BUFFER\n");

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
	case SHADER_BASIC_2D:
		vert_path = basic_2d_vsh_src;
		frag_path = basic_2d_fsh_src;
		break;
	default:
		vert_path = basic_2d_vsh_src;
		frag_path = basic_2d_fsh_src;
		break;
	}

	return shader_init_from_src( s, vert_path, frag_path );
}

void shader_use( const struct shader *s )
{
	if ( !s )
		return;

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
	if ( !s ) {
		pr_warn("ATTEMPTED TO DESTROY NULL SHADER\n");
		return;
	}

	glDeleteProgram( s->id );

	s->id = 0;
	s->u_count = 0;
}
