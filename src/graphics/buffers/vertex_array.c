#include <stdio.h>
#include <errno.h>
#include <glad/glad.h>
#include "vertex_array.h"

bool vertex_array_init( struct vertex_array *va )
{
	GLuint id;

	if ( !va )
		return false;

	id = 0;
	glGenVertexArrays( 1, &id );
	va->id = id;

	return id != 0;
}

bool vertex_array_link_attrib( struct vertex_array *va, const struct buffer *vbo,
                              const struct vertex_attrib *attrib )
{
	if ( !va || !vbo || !attrib )
		return false;

	if ( va->id <= 0 || vbo->id <= 0 )
		return false;

	glBindVertexArray( va->id );

	buffer_bind( vbo );
	glVertexAttribPointer( attrib->index, attrib->size, attrib->type,
	                       attrib->normalised, attrib->stride,
	                       (const GLvoid*)attrib->offset );
	glEnableVertexAttribArray( attrib->index );
	buffer_unbind( vbo );

	return true;
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
	if ( !va || va->id <= 0 )
		return;

	glDeleteVertexArrays( 1, &va->id );
	va->id = 0;
}
