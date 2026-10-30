#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "log.h"
#include "stb_truetype.h"

#include "font.h"

#define FONT_ATLAS_SIZE	512

void font_destroy( struct font *font )
{
	if ( !font )
		return;

	texture_destroy( &font->atlas );
	memset( font, 0, sizeof( *font ) );
}

bool font_init( struct font *font, const char *path, float pixel_height )
{
	struct texture_config config = TEXTURE_CONFIG_DEFAULT;
	stbtt_fontinfo info = { 0 };
	stbtt_bakedchar baked[FONT_GLYPH_COUNT];
	void *file_data = NULL;
	unsigned char *bitmap = NULL;
	unsigned char *pixels = NULL;
	size_t file_size;
	size_t pixel_count;
	size_t src, dst;
	int offset;
	int ascent, descent, line_gap;
	int x, y, i;
	float scale;
	bool success = false;

	if ( !font )
		return false;

	memset( font, 0, sizeof( *font ) );

	if ( !path || !path[0] || !isfinite( pixel_height ) || pixel_height <= 0.0f )
		return false;

	if ( !file_read_binary( path, &file_data, &file_size ) ) {
		pr_err("UNABLE TO READ FONT FILE\n");
		goto CLEAN;
	}

	if ( file_size < 12 ) {
		pr_err("FONT FILE IS TOO SMALL\n");
		goto CLEAN;
	}

	offset = stbtt_GetFontOffsetForIndex( file_data, 0 );
	if ( offset < 0 || !stbtt_InitFont( &info, file_data, offset ) ) {
		pr_err("UNABLE TO INITIALISE FONT\n");
		goto CLEAN;
	}

	pixel_count = (size_t)FONT_ATLAS_SIZE * FONT_ATLAS_SIZE;

	bitmap = malloc( pixel_count );
	pixels = malloc( pixel_count * 4 );
	if ( !bitmap || !pixels ) {
		pr_err("UNABLE TO ALLOCATE FONT ATLAS\n");
		goto CLEAN;
	}

	if ( stbtt_BakeFontBitmap( file_data, offset, pixel_height, bitmap,
	                           FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, FONT_FIRST_CHAR,
	                           FONT_GLYPH_COUNT, baked ) <= 0 )
	{
		pr_err("FONT CHARACTERS DO NOT FIT IN ATLAS\n");
		goto CLEAN;
	}

	scale = stbtt_ScaleForPixelHeight( &info, pixel_height );
	stbtt_GetFontVMetrics( &info, &ascent, &descent, &line_gap );

	font->pixel_height = pixel_height;
	font->ascent = (float)ascent * scale;
	font->line_height = ( (float)ascent - (float)descent + (float)line_gap ) * scale;

	for ( i = 0; i < FONT_GLYPH_COUNT; i++ ) {
		struct font_glyph *glyph = &font->glyphs[i];

		glyph->region.x = baked[i].x0;
		glyph->region.y = baked[i].y0;
		glyph->region.width = baked[i].x1 - baked[i].x0;
		glyph->region.height = baked[i].y1 - baked[i].y0;

		glyph->offset = vec2_make( baked[i].xoff, baked[i].yoff );
		glyph->advance = baked[i].xadvance;
	}

	for ( y = 0; y < FONT_ATLAS_SIZE; y++ ) {
		for ( x = 0; x < FONT_ATLAS_SIZE; x++ ) {
			src = (size_t)y * FONT_ATLAS_SIZE + (size_t)x;
			dst = ( (size_t)( FONT_ATLAS_SIZE - 1 - y ) *
			        FONT_ATLAS_SIZE + (size_t)x ) * 4;

			pixels[dst] = 255;
			pixels[dst + 1] = 255;
			pixels[dst + 2] = 255;
			pixels[dst + 3] = bitmap[src];
		}
	}

	config.min_filter = TEXTURE_LINEAR;
	config.mag_filter = TEXTURE_LINEAR;
	config.wrap = TEXTURE_CLAMP_TO_EDGE;

	if ( !texture_load_pixels( &font->atlas, pixels, FONT_ATLAS_SIZE,
	                           FONT_ATLAS_SIZE, 0, &config ) )
	{
		pr_err("UNABLE TO UPLOAD FONT ATLAS\n");
		goto CLEAN;
	}

	success = true;

CLEAN:
	free( pixels );
	free( bitmap );
	file_free( file_data );

	if ( !success )
		font_destroy( font );

	return success;
}
