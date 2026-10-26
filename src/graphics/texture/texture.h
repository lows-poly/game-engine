#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>

#include <glad/glad.h>

#include "stb_image.h"

#define TEXTURE_CONFIG_DEFAULT { \
	.type = TEXTURE_2D, \
	.format = TEXTURE_RGBA, \
	.ptype = TEXTURE_UBYTE, \
	.min_filter = TEXTURE_LINEAR_MIPMAP_LINEAR, \
	.mag_filter = TEXTURE_LINEAR, \
	.wrap = TEXTURE_REPEAT \
}

enum texture_type {
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D,
	TEXTURE_RECT,
	TEXTURE_BUF,
	TEXTURE_CUBE_MAP
};

enum texture_format {
	TEXTURE_RGB,
	TEXTURE_RGBA
};

enum texture_ptype {
	TEXTURE_UBYTE,
	TEXTURE_FLOAT
};

enum texture_filter {
	TEXTURE_NEAREST,
	TEXTURE_LINEAR,
	TEXTURE_NEAREST_MIPMAP_NEAREST,
	TEXTURE_LINEAR_MIPMAP_NEAREST,
	TEXTURE_NEAREST_MIPMAP_LINEAR,
	TEXTURE_LINEAR_MIPMAP_LINEAR
};

enum texture_wrap {
	 TEXTURE_REPEAT,
	 TEXTURE_CLAMP_TO_EDGE,
	 TEXTURE_MIRRORED_REPEAT
};

struct texture {
	GLuint id;
	GLenum type;
};

struct texture_config {
	enum texture_type	type;
	enum texture_format	format;
	enum texture_ptype	ptype;
	enum texture_filter	min_filter;
	enum texture_filter	mag_filter;
	enum texture_wrap	wrap;
};

/**
 * texture_init() - Initialise a new texture object to the empty state.
 * @texture
 * @min_filter
 * @mag_filter
 *
 * Return: True on success.
 */
bool texture_init( struct texture *texture );

/**
 * texture_load() - Load an image into an initialised, empty texture obj.
 * @texture
 * @min_filter
 * @mag_filter
 *
 * Return: True on success.
 */
bool texture_load( struct texture *texture, const char *img, unsigned int slot,
                  struct texture_config *config );

/**
 * texture_reload() - Replace a loaded texture.
 * @texture
 * @min_filter
 * @mag_filter
 *
 * On failure, the existing texture is preserved.
 *
 * Return: True on success.
 */
bool texture_reload( struct texture *texture, const char *img, unsigned int slot,
                    struct texture_config *config );

bool texture_set_filter( struct texture *texture, enum texture_filter min_filter,
                        enum texture_filter mag_filter );
bool texture_set_wrap( struct texture *texture, enum texture_wrap wrap );
void texture_bind( const struct texture *texture, unsigned int slot );
void texture_unbind( const struct texture *texture );
void texture_destroy( struct texture *texture );

#endif
