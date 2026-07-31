#include <stdio.h>
#include <errno.h>
#include "mesh.h"

static const unsigned int QUAD_INDICES[6] = { 0, 1, 2, 2, 3, 0 };
static const struct vertex_attrib QUAD_ATTRIBS[1] = {
	{
		.index = 0,
		.size = 2,
		.type = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride = sizeof( vec2 ),
		.offset = 0
	}
};

static const unsigned int TRI_INDICES[3] = { 0, 1, 2 };
static const struct vertex_attrib TRI_ATTRIBS[1] = {
	{
		.index = 0,
		.size = 2,
		.type = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride = sizeof( vec2 ),
		.offset = 0
	}
};

int mesh_init( struct mesh *m, const struct mesh_desc *desc, enum draw_type draw_type )
{
	int err;
	size_t i, index_size;

	if ( !desc || !desc->vertices || !desc->vertex_size ) {
		printf("MESH_ERR: INVALID DESC\n");
		return -EINVAL;
	}

	/* VAO */
	err = vertex_array_init( &m->vao );
	if ( err ) {
		printf("MESH_ERR: FAILED TO CREATE VAO\n");
		return err;
	}

	/* VBO */
	err = buffer_init( &m->vbo, GL_ARRAY_BUFFER, desc->vertices,
	                     desc->vertex_size, draw_type );
	if ( err ) {
		printf("MESH_ERR: FAILED TO CREATE VBO\n");
		goto clean_vao;
	}

	for ( i = 0; i < desc->attrib_count; i++ ) {
		err = vertex_array_link_attrib( &m->vao, &m->vbo, &desc->attribs[i] );
		if ( err ) {
			printf("MESH_ERR: FAILED TO LINK ATTRIB %zu\n", i);
			goto clean_vbo;
		}
	}

	m->vertex_count = desc->vertex_count;
	m->has_indices = desc->indices != NULL;
	
	/* EBO */
	if ( m->has_indices ) {
		index_size = (size_t)desc->index_count * sizeof( unsigned int );

		err = buffer_init( &m->ebo, GL_ELEMENT_ARRAY_BUFFER, desc->indices,
		                     index_size, GL_STATIC_DRAW );
		if ( err ) {
			printf("MESH_ERR: FAILED TO CREATE EBO\n");
			goto clean_vbo;
		}

		vertex_array_bind( &m->vao );
		buffer_bind( &m->ebo );
		
		m->index_count = desc->index_count;
	}

	return 0;

clean_vbo:
	buffer_destroy( &m->vbo );
clean_vao:
	vertex_array_destroy( &m->vao );
	return err;
}

int mesh_init_quad( struct mesh *m, const vec2 verts[4], enum draw_type draw_type )
{
	struct mesh_desc desc;

	if ( !m || !verts )
		return -EINVAL;

	desc.vertices = verts;
	desc.vertex_size = sizeof( vec2 ) * 4;
	desc.attribs = QUAD_ATTRIBS;
	desc.attrib_count = 1;
	desc.indices = QUAD_INDICES;
	desc.vertex_count = 4;
	desc.index_count = 6;

	return mesh_init( m, &desc, draw_type );
}

int mesh_init_tri( struct mesh *m, const vec2 verts[3], enum draw_type draw_type )
{
	struct mesh_desc desc;

	if ( !m || !verts )
		return -EINVAL;

	desc.vertices = verts;
	desc.vertex_size = sizeof( vec2 ) * 3;
	desc.attribs = TRI_ATTRIBS;
	desc.attrib_count = 1;
	desc.indices = TRI_INDICES;
	desc.vertex_count = 3;
	desc.index_count = 3;

	return mesh_init( m, &desc, draw_type );
}

int mesh_update_tri( struct mesh *m, const vec2 verts[3] )
{
	if ( !m || !verts )
		return -EINVAL;

	return mesh_update_vertices( m, verts, sizeof( vec2 ) * 3, 0 );
}

int mesh_update_quad( struct mesh *m, const vec2 verts[4] )
{
	if ( !m || !verts )
		return -EINVAL;

	return mesh_update_vertices( m, verts, sizeof( vec2 ) * 4, 0 );
}

void mesh_draw( const struct mesh *m )
{
	vertex_array_bind( &m->vao );

	if ( m->has_indices )
		glDrawElements( GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT, NULL );
	else
		glDrawArrays( GL_TRIANGLES, 0, m->vertex_count );
}

int mesh_update_vertices( struct mesh *m, const void *vertices, size_t vertex_size,
                          size_t offset )
{
	if ( !m || !vertices || !vertex_size ) {
		return -EINVAL;
	}

	buffer_update( &m->vbo, vertices, vertex_size, offset );
}

void mesh_destroy( struct mesh *m )
{
	if ( m->has_indices )
		buffer_destroy( &m->ebo );

	buffer_destroy( &m->vbo );
	vertex_array_destroy( &m->vao );
}
