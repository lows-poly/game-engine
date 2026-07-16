#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdbool.h>

struct shader {
	GLuint id;
};

bool shader_create( struct shader *s, const char *vert_path, const char *frag_path );
void shader_use( const struct shader *s );
void shader_destroy( struct shader *s );

#endif
