#include <stddef.h>

#include "renderer_2d.h"
#include "draw.h"

static struct renderer_2d *current_renderer;
static const struct font *current_font;

void draw_set_renderer( struct renderer_2d *renderer )
{
	current_renderer = renderer;
}

void draw_set_font( const struct font *font )
{
	current_font = font;
}

void draw_reset( void )
{
	current_renderer = NULL;
	current_font = NULL;
}

void draw_text( const char *text, float x, float y, float scale, colour tint )
{
	if ( !current_renderer || !current_font )
		return;

	renderer_2d_draw_text( current_renderer, current_font, text,
	                       x, y, scale, tint );
}

void draw_shape( const struct shape2d *shape )
{
	if ( !current_renderer )
		return;

	renderer_2d_draw_shape( current_renderer, shape );
}

void draw_texture( const struct shape2d *shape, const struct texture *texture,
                   colour tint )
{
	if ( !current_renderer )
		return;

	renderer_2d_draw_texture( current_renderer, shape, texture, tint );
}

void draw_texture_region( const struct shape2d *shape, const struct texture *texture,
                          const struct texture_region *region, colour tint )
{
	if ( !current_renderer )
		return;

	renderer_2d_draw_texture_region( current_renderer, shape, texture,
	                                 region, tint );
}

void draw_sprite( const struct sprite *sprite )
{
	if ( !current_renderer )
		return;

	renderer_2d_draw_sprite( current_renderer, sprite );
}
