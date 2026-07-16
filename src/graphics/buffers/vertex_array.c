#include <stdio.h>
#include <errno.h>
#include <glad/glad.h>
#include "vertex_array.h"

int vertex_array_create( struct vertex_array *va )
{
	GLuint id;

	if ( !va )
		return -EINVAL;

	glGenVertexArrays( 1, &id );
	va->id = id;

	return 0;
}

int vertex_array_link_attrib( struct vertex_array *va, const struct buffer *vbo,
                              struct vertex_attrib *attrib )
{
	if ( !va || !vbo ) {
		printf("VERTEX_ARRAY_ERR: INVALID ARGS\n");
		return -EINVAL;
	}

	glBindVertexArray( va->id );
	buffer_bind( vbo );
	glVertexAttribPointer( attrib->index, attrib->size, attrib->type,
	                       attrib->normalised, attrib->stride,
	                       (const GLvoid*)attrib->offset );
	glEnableVertexAttribArray( attrib->index );
	buffer_unbind( vbo );

	return 0;
}

void vertex_array_bind( const struct vertex_array *va )
{
	glBindVertexArray( va->id );
}

void vertex_array_unbind( void )
{
	glBindVertexArray( 0 );
}

void vertex_array_destroy( struct vertex_array *va )
{
	if ( va->vao )
		glDeleteVertexArrays( 1, &va->id );
}
