#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <stdbool.h>
#include <glad/glad.h>
#include "buffer.h"

struct vertex_array {
	GLuint id;
};

struct vertex_attrib {
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalised;
	GLsizei stride;
	size_t offset;
};

int vertex_array_create( struct vertex_array *va );
int vertex_array_link_attrib( struct vertex_array *va, const struct buffer *vbo,
                              const struct vertex_attrib *attrib );
void vertex_array_bind( const struct vertex_array *va );
void vertex_array_unbind( void );
void vertex_array_destroy( struct vertex_array *va );

#endif
