#ifndef ENGINE_DRAW_H
#define ENGINE_DRAW_H

#include "primitives/colour.h"

struct renderer_2d;
struct font;
struct shape2d;
struct texture;
struct texture_region;
struct sprite;

/**
 * draw_set_renderer() - Select the renderer used by convenience drawing calls.
 * Passing NULL clears the corresponding selection.
 * @renderer: Pointer to renderer.
 *
 * Objects remain owned by the caller and must stay alive while selected.
 */
void draw_set_renderer( struct renderer_2d *renderer );
void draw_set_font( const struct font *font );

void draw_reset( void );
void draw_text( const char *text, float x, float y, float scale, colour tint );
void draw_shape( const struct shape2d *shape );
void draw_texture( const struct shape2d *shape, const struct texture *texture,
                   colour tint );
void draw_texture_region( const struct shape2d *shape, const struct texture *texture,
                          const struct texture_region *region, colour tint );
void draw_sprite( const struct sprite *sprite );

#endif
