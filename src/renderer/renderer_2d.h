#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include <stdbool.h>

#include "graphics/shader/shader.h"
#include "window/window.h"
#include "primitives/shape2d.h"
#include "primitives/colour.h"

struct renderer_2d {
	struct shader default_shader;
	struct shader *shader;
	int width;
	int height;
};

/*
 * renderer_2d_init() - Init shader 2d preset and set projection.
 * @renderer_2d struct
 * @width
 * @height
 *
 * return: 0 on success, failure otherwise.
 */
int renderer_2d_init( struct renderer_2d *r, int width, int height );
void renderer_2d_set_shader( struct renderer_2d *r, struct shader *s );
void renderer_2d_update( struct renderer_2d *r, const struct window *w );
void renderer_2d_draw( struct renderer_2d *r, struct shape2d *shape );
int renderer_2d_resize( struct renderer_2d *r, int width, int height );
void renderer_2d_destroy( struct renderer_2d *r );

#endif
