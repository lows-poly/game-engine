#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <glad/glad.h>

#include "shader.h"
#include "path.h"
#include "file.h"

static GLuint shader_compile( GLenum type, const char *src )
{
	int success;
	GLuint id = glCreateShader( type );
	
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

	if ( !file_read_text_rel( vert_path, &vert_src ) ) {
		printf("SHADER_ERR: FAILED TO READ %s\n", vert_path);
		return -EINVAL;
	}

	if ( !file_read_text_rel( frag_path, &frag_src ) ) {
		printf("SHADER_ERR: FAILED TO READ %s\n", frag_path);
		file_free( vert_src );
		return -EINVAL;
	}

	GLuint vert = shader_compile( GL_VERTEX_SHADER, vert_src );
	GLuint frag = shader_compile( GL_FRAGMENT_SHADER, frag_src );

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
	int success;
	GLuint id = glCreateProgram();
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

	glDeleteShader(	vert );
	glDeleteShader( frag );

	s->id = id;
	return 0;
}

void shader_use( const struct shader *s )
{
	glUseProgram( s->id );
}

void shader_destroy( struct shader *s )
{
	GLuint id = s->id;

	if ( id )
		glDeleteProgram( id );
}
