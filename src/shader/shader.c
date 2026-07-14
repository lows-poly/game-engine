#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "shader.h"
#include "path.h"

static GLuint shader_compile( GLenum type, const char *src )
{
	GLuint id = glCreateShader( type );
	
	glShaderSource( id, 1, &src, NULL );
	glCompileShader( id );

	int success;
	glGetShaderiv( id, GL_COMPILE_STATUS, &success );

	if ( !success ) {
		char info_log[512];

		glGetShaderInfoLog( id, 512, NULL, info_log );

		switch ( type ) {
		case GL_VERTEX_SHADER:
			printf("SHADER_COMPILATION_ERR: VERTEX_FAILED: ");
		case GL_FRAGMENT_SHADER:
			printf("SHADER_COMPILATION_ERR: FRAGMENT_FAILED: ");
		default:
			printf("SHADER_COMPILATION_ERR: UNKNOWN TYPE: ");
		};

		printf("%s\n", info_log);
	}

	return id;
}

bool shader_create( struct shader *s, const char *vert_path, const char *frag_path )
{
	char *vert_src = vert_path;
	char *frag_src = frag_path;

	if ( !vert_src || !frag_src ) {
		goto clean_shader_src;
		return false;
	}

	GLuint vert = shader_compile( GL_VERTEX_SHADER, vert_src );
	GLuint frag = shader_compile( GL_FRAGMENT_SHADER, frag_src );
	goto clean_shader_src;

	if ( !vert || !frag ) {
		glDeleteShader( vert );
		glDeleteShader( frag );
		return false;
	}

	// SHADER PROGRAM
	GLuint id = glCreateProgram();
	glAttachShader( id, vert );
	glAttachShader( id, frag );
	glLinkProgram( id );

	clean_shader_src:
		free( vert_src );
		free( frag_src );
	
	int success;
	glGetProgramiv( id, GL_LINK_STATUS, &success );

	if ( !success ) {
		char info_log[512];
		glGetShaderInfoLog( id, 512, NULL, info_log );
		printf("SHADER_ERR: PROGRAM_LINKING_FAILED: %s\n", info_log);
		return false;
	}

	s->id = id;
	return true;
}

void shader_use( GLuint shader_id )
{
	glUseProgram( shader_id );
}

void shader_destroy( struct shader *s )
{
	glDeleteProgram( s->id );
}
