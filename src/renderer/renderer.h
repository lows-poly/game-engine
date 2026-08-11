#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <glad/glad.h>

#include "graphics/shader/shader.h"
#include "graphics/mesh.h"
#include "primitives/colour.h"

enum draw_order {
	DRAW_ORDER_CW = GL_CW,
	DRAW_ORDER_CCW = GL_CCW
};

/* 
 * renderer_begin_frame() - Call glClear and glClearColor.
 * @c: colour primitive
 *
 * Convert colour primitive to float
 */
void renderer_begin_frame( colour c );

/* 
 * renderer_enable_backface_culling() - Enable backface culling.
 * @value: bool
 * @order: front face draw order
 */
void renderer_enable_backface_culling( bool value, enum draw_order order );

/*
 * renderer_draw_mesh() - Use shader and draw mesh.
 * @m: mesh
 * @s: shader
 */
void renderer_draw_mesh( const struct mesh *m, const struct shader *s );

/*
 * renderer_set_2d_projection()
 * @shader
 * @width
 * @height
 *
 * Return: 0 on success, otherwise failure.
 */
int renderer_set_2d_projection( struct shader *s, int width, int height );

#endif
