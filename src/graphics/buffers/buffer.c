#include <stdio.h>
#include <errno.h>
#include "buffer.h"

int buffer_init( struct buffer *b, GLenum target, const void *data, size_t size,
                 GLenum usage )
{
	GLuint id;

	if ( !b || !data )
		return -EINVAL;

	glGenBuffers( 1, &id );

	if ( id <= 0 )
		return -EINVAL;

	glBindBuffer( target, id );
	glBufferData( target, size, data, usage );

	b->id = id;
	b->target = target;

	return 0;
}
void buffer_update( const struct buffer *b, const void *data, size_t size,
                    size_t offset )
{
	glBindBuffer( b->target, b->id );
	glBufferSubData( b->target, offset, size, data );
}

void buffer_bind( const struct buffer *b )
{
	glBindBuffer( b->target, b->id );
}

void buffer_unbind( const struct buffer *b )
{
	glBindBuffer( b->target, 0 );
}

void buffer_destroy( struct buffer *b )
{
	if ( !b || b->id <= 0 )
		return;

	glDeleteBuffers( 1, &b->id );
	b->id = 0;
}
