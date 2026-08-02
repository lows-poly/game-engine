#ifndef ENGINE_SHAPE2D_H
#define ENGINE_SHAPE2D_H

#include "graphics/mesh.h"
#include "graphics/shader/shader.h"
#include "primitives/colour.h"
#include "primitives/vec2.h"

enum shape_type {
	SHAPE2D_RECTANGLE,
	SHAPE2D_TRIANGLE
};

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
 * shape2d_init() - Initialise a created shaoe struct.
 * @shape: shape_type enum
 * @s:     struct shape2d
 *
 * Return:
 * 1 - Success
 * Otherwise - Failure
 */
int shape2d_init( enum shape_type shape, struct shape2d *s );

/*
 * shape2d_create()
 * @shape
 * @shader
 * @x
 * @y
 * @width
 * @height
 * @colour
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int shape2d_create( struct shape2d *s, struct shader *shader, float x, float y,
                    float w, float h, colour c );

/*
 * shape2d_move() - Move x, y coordinate
 * @s
 * @dx
 * @dy
 */
void shape2d_move( struct shape2d *s, float dx, float dy );

/*
 * shape2d_set_pos() - Set x, y coordinate
 * @s
 * @x
 * @y
 */
void shape2d_set_pos( struct shape2d *s, float x, float y );

/*
 * shape2d_set_colour() - Set uniform colour
 * @s: struct shape2d
 * @c: colour
 */
void shape2d_set_colour( struct shape2d *s, colour c );

/*
 * shape2d_draw() - Draw mesh and update uniform properties
 * @s
 */
void shape2d_draw( const struct shape2d *s );

/*
 * shape2d_destroy() - Destroy mesh
 * @s
 */
void shape2d_destroy( struct shape2d *s );

#endif
