#include <stdio.h>
#include <errno.h>
#include "mesh.h"

int mesh_create( struct mesh *m, const void *vertices, size_t vertex_size,
                 GLsizei vertex_count, const struct vertex_attrib *attribs,
                 size_t attrib_count, const unsigned int *indices,
                 GLsizei index_count )
{
	int err;

	// VAO
	err = vertex_array_create( &m->vao );
	if ( err ) {
		printf("MESH_ERR: FAILED TO CREATE VAO\n");
		return err;
	}

	// VBO
	err = buffer_create( &m->vbo, GL_ARRAY_BUFFER, vertices, vertex_size,
	                     GL_STATIC_DRAW );
	if ( err ) {
		printf("MESH_ERR: FAILED TO CREATE VBO\n");
		goto clean_vao;
	}

	for ( size_t i = 0; i < attrib_count; i++ ) {
		err = vertex_array_link_attrib( &m->vao, &m->vbo, &attribs[i] );
		if ( err ) {
			printf("MESH_ERR: FAILED TO LINK ATTRIB %zu\n", i);
			goto clean_vbo;
		}
	}

	m->vertex_count = vertex_count;
	m->has_indices = indices != NULL;
	
	// EBO
	if ( m->has_indices ) {
		size_t index_size = (size_t)index_count * sizeof( unsigned int );

		// CREATE EBO
		err = buffer_create( &m->ebo, GL_ELEMENT_ARRAY_BUFFER, indices,
		                     index_size, GL_STATIC_DRAW );
		if ( err ) {
			printf("MESH_ERR: FAILED TO CREATE EBO\n");
			goto clean_vbo;
		}

		// BIND EBO
		vertex_array_bind( &m->vao );
		buffer_bind( &m->ebo );
		
		m->index_count = index_count;
	}

	return 0;

clean_vbo:
	buffer_destroy( &m->vbo );
clean_vao:
	vertex_array_destroy( &m->vao );
	return err;
}

void mesh_draw( const struct mesh *m )
{
	vertex_array_bind( &m->vao );

	if ( m->has_indices )
		glDrawElements( GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT, NULL );
	else
		glDrawArrays( GL_TRIANGLES, 0, m->vertex_count );
}

void mesh_destroy( struct mesh *m )
{
	if ( m->has_indices )
		buffer_destroy( &m->ebo );

	buffer_destroy( &m->vbo );
	vertex_array_destroy( &m->vao );
}
