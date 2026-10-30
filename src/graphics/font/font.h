#ifndef ENGINE_FONT_H
#define ENGINE_FONT_H

#include <stdbool.h>

#include "graphics/texture/texture.h"
#include "primitives/vec2.h"

#define FONT_FIRST_CHAR		32
#define FONT_LAST_CHAR		126
#define FONT_GLYPH_COUNT	( FONT_LAST_CHAR - FONT_FIRST_CHAR + 1 )

struct font_glyph {
	struct texture_region region;
	vec2 offset;
	float advance;
};

struct font {
	struct texture atlas;
	struct font_glyph glyphs[FONT_GLYPH_COUNT];

	float pixel_height;
	float line_height;
	float ascent;
};

bool font_init( struct font *font, const char *path, float pixel_height,
                const struct texture_config *config );
void font_destroy( struct font *font );

#endif
