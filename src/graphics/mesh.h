#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <stdbool.h>
#include <stddef.h>

#include "buffers/buffer.h"
#include "buffers/vertex_array.h"
#include "primitives/vec2.h"

#include "debugf.h"

struct mesh {
	struct vertex_array vao;
	struct buffer vbo;
	struct buffer ebo;
	GLsizei vertex_count;
	GLsizei index_count;
	bool has_indices;
};

static const struct field_desc mesh_fields[] = {
	FIELD( struct mesh, vertex_count, FIELD_INT ),
	FIELD( struct mesh, index_count, FIELD_INT ),
	FIELD( struct mesh, has_indices, FIELD_BOOL )
};

#define MESH_FIELD_COUNT ( sizeof( mesh_fields ) / sizeof( mesh_fields[0] ) )

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
int mesh_init_quad( struct mesh *m, const vec2 verts[4], enum draw_type draw_type );
int mesh_init_tri( struct mesh *m, const vec2 verts[3], enum draw_type draw_type );
int mesh_update_tri( struct mesh *m, const vec2 verts[3] );
int mesh_update_quad( struct mesh *m, const vec2 verts[4] );
int mesh_update_vertices( struct mesh *m, const void *vertices, size_t vertex_size,
                          size_t offset );
void mesh_draw( const struct mesh *m );
void mesh_destroy( struct mesh *m );

#endif
