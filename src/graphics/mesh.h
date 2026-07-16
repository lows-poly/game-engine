#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <stdbool.h>
#include <stddef.h>

#include "buffers/buffer.h"
#include "buffers/vertex_array.h"

struct mesh {
	struct vertex_array vao;
	struct buffer vbo;
	struct buffer ebo;
	GLsizei vertex_count;
	GLsizei index_count;
	bool has_indices;
};

int mesh_create( struct mesh *m, const void *vertices, size_t vertex_size,
                  GLsizei vertex_count, const struct vertex_attrib *attribs,
                  size_t attrib_count, const unsigned int *indices,
                  GLsizei index_count );
void mesh_draw( const struct mesh *m );
void mesh_destroy( struct mesh *m );

#endif
