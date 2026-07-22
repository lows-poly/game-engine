#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "shader.h"
#include "path.h"
#include "file.h"

static void shader_cache_uniform( struct shader *s, const char *name, GLint loc )
{
	size_t len;

	assert( s != NULL );
	assert( name != NULL );

	if ( loc < 0 )
		return;

	if ( s->u_count >= SHADER_UNIFORM_CACHE_MAX )
		return;

	len = strlen( name );
	if ( len >= SHADER_UNIFORM_NAME_MAX ) {
		printf("SHADER_WARN: UNIFORM NAME TOO LONG, NOT CACHED: %s\n", name);
		return;
	}

	snprintf( s->uniforms[s->u_count].name, SHADER_UNIFORM_NAME_MAX,
	          "%s", name );

	s->uniforms[s->u_count].loc = loc;
	s->u_count++;
}

static GLint shader_get_uniform_loc( struct shader *s, const char *name )
{
	GLint loc;

	assert( s != NULL );
	assert( name != NULL );
	assert( s->id != 0 );

	for ( size_t i = 0; i < s->u_count; i++ ) {
		struct shader_uniform *u = &s->uniforms[i];

		if ( strcmp( u->name, name ) == 0 )
			return u->loc;
	}

	loc = glGetUniformLocation( s->id, name );

	shader_cache_uniform( s, name, loc );

	return loc;
}

static GLuint shader_compile( GLenum type, const char *src )
{
	int success;
	GLuint id;

	assert( src != NULL );

	id = glCreateShader( type );

	if ( id == 0 ) {
		printf("SHADER_ERR: glCreateShader FAILED FOR TYPE %d\n", (int)type);
		return;
	}
	
	glShaderSource( id, 1, &src, NULL );
	glCompileShader( id );
	glGetShaderiv( id, GL_COMPILE_STATUS, &success );

	if ( !success ) {
		char info_log[512];
		glGetShaderInfoLog( id, 512, NULL, info_log );

		switch ( type ) {
		case GL_VERTEX_SHADER:
			printf("SHADER_COMPILATION_ERR: VERTEX_FAILED: ");
			break;
		case GL_FRAGMENT_SHADER:
			printf("SHADER_COMPILATION_ERR: FRAGMENT_FAILED: ");
			break;
		default:
			printf("SHADER_COMPILATION_ERR: UNKNOWN TYPE: ");
			break;
		};

		printf("%s\n", info_log);
		glDeleteShader( id );
		return 0;
	}

	return id;
}

int shader_create( struct shader *s, const char *vert_path, const char *frag_path )
{
	char *vert_src = NULL;
	char *frag_src = NULL;
	GLuint vert, frag;
	GLuint id;
	int success;

	assert( s != NULL );
	assert( vert_path != NULL );
	assert( frag_path != NULL );

	if ( !file_read_text_rel( vert_path, &vert_src ) ) {
		printf("SHADER_ERR: FAILED TO READ %s\n", vert_path);
		return -EINVAL;
	}

	if ( !file_read_text_rel( frag_path, &frag_src ) ) {
		printf("SHADER_ERR: FAILED TO READ %s\n", frag_path);
		file_free( vert_src );
		return -EINVAL;
	}

	if ( vert_src == NULL || frag_src == NULL ) {
		printf("SHADER_ERR: FILE READ REPORTED SUCCESS WITH NULL BUFFER\n");
		file_free( vert_src );
		file_free( frag_src );
		return -EINVAL;
	}

	vert = shader_compile( GL_VERTEX_SHADER, vert_src );
	frag = shader_compile( GL_FRAGMENT_SHADER, frag_src );

	file_free( vert_src );
	file_free( frag_src );

	if ( !vert || !frag ) {
		if ( vert )
			glDeleteShader( vert );

		if ( frag )
			glDeleteShader( frag );

		return -EINVAL;
	}

	// SHADER PROGRAM
	id = glCreateProgram();

	if ( id == 0 ) {
		printf("SHADER_ERR: glCreateProgram FAILED\n");
		glDeleteShader( vert );
		glDeleteShader( frag );
		return -ENOMEM;
	}

	glAttachShader( id, vert );
	glAttachShader( id, frag );
	glLinkProgram( id );

	glGetProgramiv( id, GL_LINK_STATUS, &success );
	if ( !success ) {
		char info_log[512];

		glGetProgramInfoLog( id, 512, NULL, info_log );
		printf("SHADER_ERR: PROGRAM_LINKING_FAILED: %s\n", info_log);

		glDeleteShader( vert );
		glDeleteShader( frag );
		glDeleteProgram( id );

		return -EINVAL;
	}

	glDetachShader( id, vert );
	glDetachShader( id, frag );
	glDeleteShader(	vert );
	glDeleteShader( frag );

	s->id = id;
	s->u_count = 0;

	return 0;
}

void shader_use( const struct shader *s )
{
	assert( s != NULL );
	assert( s->id != 0 );

	glUseProgram( s->id );
}

int shader_set_int( struct shader *s, const char *name, int value )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return -EINVAL;

	glUniform1i( loc, value );
	
	return 0;
}

int shader_set_float( struct shader *s, const char *name, float value )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return -EINVAL;

	glUniform1f( loc, value );
	
	return 0;
}

int shader_set_3i( struct shader *s, const char *name, const int value[3] )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return -EINVAL;

	glUniform3iv( loc, 1, value );
	
	return 0;
}

int shader_set_ivec3( struct shader *s, const char *name, ivec3 v )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return -EINVAL;

	glUniform3i( loc, v.x, v.y, v.z );
	
	return 0;
}

int shader_set_3f( struct shader *s, const char *name, const float value[3] )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return -EINVAL;

	glUniform3fv( loc, 1, value );
	
	return 0;
}

int shader_set_vec3( struct shader *s, const char *name, vec3 v )
{
	GLint loc;

	loc = shader_get_uniform_loc( s, name );

	if ( loc < 0 )
		return -EINVAL;

	glUniform3f( loc, v.x, v.y, v.z );
	
	return 0;
}

void shader_destroy( struct shader *s )
{
	assert( s != NULL );
	assert( s->id != 0 );

	GLuint id = s->id;

	glDeleteProgram( id );

	s->id = 0;
	s->u_count = 0;
}
