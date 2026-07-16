#include <stdio.h>
#include <errno.h>
#include "buffer.h"

int buffer_create( struct buffer *b, GLenum target, const void *data, size_t size,
                   GLenum usage )
{
	GLuint id;

	if ( !data || !size ) {
		printf("BUFFER_ERR: INVALID DATA\n");
		return -EINVAL;
	}

	glGenBuffers( 1, &id );
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
	GLuint id = b->id;

	if ( id )
		glDeleteBuffers( 1, &id );
}
