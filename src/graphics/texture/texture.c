#include "texture.h"

#include "log.h"

static GLenum _texture_type_to_gl( enum texture_type type )
{
	switch ( type ) {
	case TEXTURE_1D:
		return GL_TEXTURE_1D;
	case TEXTURE_2D:
		return GL_TEXTURE_2D;
	case TEXTURE_3D:
		return GL_TEXTURE_3D;
	case TEXTURE_RECT:
		return GL_TEXTURE_RECTANGLE;
	case TEXTURE_BUF:
		return GL_TEXTURE_BUFFER;
	case TEXTURE_CUBE_MAP:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return 0;
	}
}

static GLenum _texture_format_to_gl( enum texture_format format )
{
	switch ( format ) {
	case TEXTURE_RGB:
		return GL_RGB;
	case TEXTURE_RGBA:
		return GL_RGBA;
	default:
		return 0;
	}
}

static GLenum _texture_ptype_to_gl( enum texture_ptype ptype )
{
	switch ( ptype ) {
	case TEXTURE_UBYTE:
		return GL_UNSIGNED_BYTE;
	case TEXTURE_FLOAT:
		return GL_FLOAT;
	default:
		return 0;
	}
}

static GLenum _texture_filter_to_gl( enum texture_filter filter )
{
	switch ( filter ) {
	case TEXTURE_NEAREST:
		return GL_NEAREST;
	case TEXTURE_LINEAR:
		return GL_LINEAR;
	case TEXTURE_NEAREST_MIPMAP_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case TEXTURE_LINEAR_MIPMAP_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case TEXTURE_NEAREST_MIPMAP_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case TEXTURE_LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		return 0;
	}
}

static GLenum _texture_wrap_to_gl( enum texture_wrap wrap )
{
	switch ( wrap ) {
	case TEXTURE_REPEAT:
		return GL_REPEAT;
	case TEXTURE_CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	case TEXTURE_MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	default:
		return 0;
	}
}

static int texture_load_new( struct texture *texture, const char *img,
                             unsigned int slot, struct texture_config *config )
{
	GLuint id;
	GLint max_slots;
	int width, height, nr_ch;
	int channels;
	GLenum type, format, ptype;
	GLenum min_filter, mag_filter, wrap;
	unsigned char *data;
	enum texture_filter mag_filter_cfg;

	if ( !texture )
		return 0;

	texture->id = 0;
	texture->type = 0;

	if ( !img || !config )
		return 0;

	mag_filter_cfg = config->mag_filter;

	if ( config->type != TEXTURE_2D ) {
		pr_err("FAILED TO INIT TEXTURE: UNSUPPORTED TEXTURE TYPE\n");
		return 0;
	}

	if ( config->ptype != TEXTURE_UBYTE ) {
		pr_err("FAILED TO INIT TEXTURE: UNSUPPORTED PIXEL TYPE\n");
		return 0;
	}

	if ( mag_filter_cfg != TEXTURE_NEAREST &&
	     mag_filter_cfg != TEXTURE_LINEAR )
	{
		pr_warn("INVALID TEXTURE MAG_FILTER, USING LINEAR FILTER\n");
		mag_filter_cfg = TEXTURE_LINEAR;
	}

	type = _texture_type_to_gl( config->type );
	format = _texture_format_to_gl( config->format );
	ptype = _texture_ptype_to_gl( config->ptype );
	min_filter = _texture_filter_to_gl( config->min_filter );
	mag_filter = _texture_filter_to_gl( mag_filter_cfg );
	wrap = _texture_wrap_to_gl( config->wrap );

	if ( !type || !ptype || !format || !min_filter || !mag_filter || !wrap ) {
		pr_err("FAILED TO INIT TEXTURE: INVALID CONFIG\n");
		return 0;
	}

	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_slots );

	if ( max_slots <= 0 || slot >= (unsigned int)max_slots ) {
		pr_err("FAILED TO INIT TEXTURE: EXCEED MAXIMUM TEXTURE SLOTS\n");
		return 0;
	}

	switch ( config->format ) {
	case TEXTURE_RGB:
		channels = 3;
		break;
	case TEXTURE_RGBA:
		channels = 4;
		break;
	default:
		pr_err("FAILED TO INIT TEXTURE: INVALID FORMAT\n");
		return 0;
	}

	stbi_set_flip_vertically_on_load( 1 );

	data = stbi_load( img, &width, &height, &nr_ch, channels );
	if ( !data ) {
		pr_err("UNABLE TO LOAD TEXTURE IMAGE\n");
		return 0;
	}

	id = 0;
	glGenTextures( 1, &id );
	if ( !id )
		goto ERR_FREE_EXIT;

	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( type, id );

	/* FILTERING */
	glTexParameteri( type, GL_TEXTURE_MIN_FILTER, (GLint)min_filter );
	glTexParameteri( type, GL_TEXTURE_MAG_FILTER, (GLint)mag_filter );

	/* WRAPPING */
	glTexParameteri( type, GL_TEXTURE_WRAP_S, (GLint)wrap );
	glTexParameteri( type, GL_TEXTURE_WRAP_T, (GLint)wrap );

	/* UPLOAD IMAGE */
	glTexImage2D( type, 0, (GLint)format, width, height, 0, format, ptype, data );

	if ( config->min_filter != TEXTURE_NEAREST &&
	     config->min_filter != TEXTURE_LINEAR )
		glGenerateMipmap( type );

	stbi_image_free( data );
	glBindTexture( type, 0 );

	texture->id = id;
	texture->type = type;

	return 1;

ERR_FREE_EXIT:
	stbi_image_free( data );
	return 0;
}

/**
 * texture_init() - Initialise a new texture object to the empty state.
 * @texture
 * @min_filter
 * @mag_filter
 *
 * Return: True on success.
 */
bool texture_init( struct texture *texture )
{
	if ( !texture )
		return false;

	texture->id = 0;
	texture->type = 0;

	return true;
}

/**
 * texture_load() - Load an image into an initialised, empty texture obj.
 * @texture
 * @min_filter
 * @mag_filter
 *
 * Return: True on success.
 */
bool texture_load( struct texture *texture, const char *img, unsigned int slot,
                  struct texture_config *config )
{
	struct texture loaded = { 0 };

	if ( !texture || texture->id )
		return false;

	if ( !texture_load_new( &loaded, img, slot, config ) )
		return false;

	*texture = loaded;
	return true;
}

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
                    struct texture_config *config )
{
	struct texture loaded = { 0 };
	GLuint old_id;

	if ( !texture || !texture->id )
		return false;

	if ( !texture_load_new( &loaded, img, slot, config ) )
		return false;

	old_id = texture->id;
	glDeleteTextures( 1, &old_id );
	*texture = loaded;

	return true;
}

bool texture_set_filter( struct texture *texture, enum texture_filter min_filter,
                        enum texture_filter mag_filter )
{
	GLenum gl_min_filter;
	GLenum gl_mag_filter;

	if ( !texture || !texture->id )
		return false;

	if ( mag_filter != TEXTURE_NEAREST &&
	     mag_filter != TEXTURE_LINEAR )
	{
		return false;
	}

	gl_min_filter = _texture_filter_to_gl( min_filter );
	gl_mag_filter = _texture_filter_to_gl( mag_filter );

	if ( !gl_min_filter || !gl_mag_filter )
		return false;

    glBindTexture( texture->type, texture->id );

    glTexParameteri( texture->type, GL_TEXTURE_MIN_FILTER, (GLint)gl_min_filter );
    glTexParameteri( texture->type, GL_TEXTURE_MAG_FILTER, (GLint)gl_mag_filter );

    if ( min_filter != TEXTURE_NEAREST &&
         min_filter != TEXTURE_LINEAR )
    {
	    glGenerateMipmap( texture->type );
    }

    glBindTexture( texture->type, 0 );

    return true;
}

bool texture_set_wrap( struct texture *texture, enum texture_wrap wrap )
{
	GLenum gl_wrap;

	if ( !texture || !texture->id )
		return false;

	gl_wrap = _texture_wrap_to_gl( wrap );

	if ( !gl_wrap )
		return false;

	glBindTexture( texture->type, texture->id );

	glTexParameteri( texture->type, GL_TEXTURE_WRAP_S, (GLint)gl_wrap );
	glTexParameteri( texture->type, GL_TEXTURE_WRAP_T, (GLint)gl_wrap );

	glBindTexture( texture->type, 0 );

	return true;
}

void texture_bind( const struct texture *texture, unsigned int slot )
{
	 GLint max_slots;

	if ( !texture || !texture->id )
		return;

	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_slots );

	if ( slot >= (unsigned int)max_slots )
		return;

	glActiveTexture (GL_TEXTURE0 + slot );
	glBindTexture( texture->type, texture->id );
}

void texture_unbind( const struct texture *texture )
{
	if ( !texture || !texture->id )
		return;

	glBindTexture( texture->type, 0 );
}

void texture_destroy( struct texture *texture )
{
	if ( !texture )
		return;

	if ( texture->id )
		glDeleteTextures( 1, &texture->id );

	texture->id = 0;
	texture->type = 0;
}
