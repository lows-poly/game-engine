#include <stdio.h>
#include <errno.h>
#include "buffer.h"

bool buffer_init( struct buffer *b, GLenum target, const void *data, size_t size,
                 GLenum usage )
{
	GLuint id;

	if ( !b || !data || size <= 0 )
		return false;

	glGenBuffers( 1, &id );

	if ( id <= 0 )
		return false;

	glBindBuffer( target, id );
	glBufferData( target, size, data, usage );

	b->id = id;
	b->target = target;
	b->size = size;

	return true;
}
bool buffer_update( const struct buffer *b, const void *data, size_t size,
                    size_t offset )
{
	if ( !b || !data || size <= 0 )
		return false;

	if ( offset > b->size || size > b->size - offset )
		return false;

	glBindBuffer( b->target, b->id );
	glBufferSubData( b->target, offset, size, data );

	return true;
}

void buffer_bind( const struct buffer *b )
{
	if ( !b )
		return;

	glBindBuffer( b->target, b->id );
}

void buffer_unbind( const struct buffer *b )
{
	if ( !b )
		return;

	glBindBuffer( b->target, 0 );
}

void buffer_destroy( struct buffer *b )
{
	if ( !b || b->id <= 0 )
		return;

	glDeleteBuffers( 1, &b->id );
	b->id = 0;
}
