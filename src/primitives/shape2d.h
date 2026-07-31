#ifndef ENGINE_SHAPE2D_H
#define ENGINE_SHAPE2D_H

#include "graphics/mesh.h"
#include "graphics/shader/shader.h"
#include "primitives/colour.h"
#include "primitives/vec2.h"

/*
 * struct shape2d - A 2 dimentional shape
 * @mesh
 * @shader
 * @pos:    vec2
 * @scale:  vec2
 * @colour: colour-float
 */
struct shape2d {
	struct mesh mesh;
	struct shader *shader;
	vec2 pos;
	vec2 scale;
	colourf colour;
};

/*
 * shape2d_rect_create() - Create a 2D rectangle
 * @s:      struct shape2d
 * @shader: struct shader
 * @x:      float
 * @y:      float
 * @w:      float
 * @h:      float
 * @c:      colour(RGB)
 */
int shape2d_rect_create( struct shape2d *s, struct shader *shader, float x, float y,
                         float w, float h, colour c );
/*
 * shape2d_rect_create() - Create a 2D rectangle
 * @s:      struct shape2d
 * @shader: struct shader
 * @x:      float
 * @y:      float
 * @w:      float
 * @h:      float
 * @c:      colour(RGB)
 */
int shape2d_tri_create( struct shape2d *s, struct shader *shader, float x, float y,
                        float w, float h, colour c );

void shape_move( struct shape2d *s, float dx, float dy );
void shape_set_pos( struct shape2d *s, float x, float y );
void shape_set_colour( struct shape2d *s, colour c );

void shape_draw( const struct shape2d *s );
void shape_destroy( struct shape2d *s );

#endif
