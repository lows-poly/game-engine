#ifndef BUFFER_H
#define BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <glad/glad.h>

struct buffer {
	GLuint id;
	GLenum target;
};

int buffer_create( struct buffer *b, GLenum target, const void *data, size_t size,
                   GLenum usage );
void buffer_update( const struct buffer *b, const void *data, size_t size,
                    size_t offset );
void buffer_bind( const struct buffer *b );
void buffer_unbind( const struct buffer *b );
void buffer_destroy( struct buffer *b );

#endif
