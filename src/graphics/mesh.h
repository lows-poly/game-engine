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

struct mesh_desc {
	const void *vertices;
	size_t vertex_size;
	const struct vertex_attrib *attribs;
	size_t attrib_count;
	const unsigned int *indices;
	GLsizei vertex_count;
	GLsizei index_count;
};

enum draw_type {
	DRAW_STATIC = GL_STATIC_DRAW,
	DRAW_DYNAMIC = GL_DYNAMIC_DRAW
};

int mesh_init( struct mesh *m, const struct mesh_desc *desc, enum draw_type draw_type );
void mesh_draw( const struct mesh *m );
void mesh_update_vertices( struct mesh *m, const void *vertices, size_t vertex_size,
                           size_t offset );
void mesh_destroy( struct mesh *m );

#endif
