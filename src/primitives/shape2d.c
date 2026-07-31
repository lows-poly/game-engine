#include <errno.h>

#include "shape2d.h"
#include "renderer/renderer.h"

static const vec2 UNIT_QUAD[4] = {
	{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
};

static const vec2 UNIT_TRI[3] = {
	{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f }
};

static int init_common( struct shape2d *s, struct shader *shader, float x,
                        float y, float w, float h, colour c )
{
	if ( !s || !shader )
		return -EINVAL;

	s->shader = shader;
	s->pos = vec2_make( x, y );
	s->scale = vec2_make( w, h );
	s->colour = colourf_make( c );

	return 0;
}

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
int shape2d_rect_create( struct shape2d *s, struct shader *shader, float x,
                         float y, float w, float h, colour c )
{
	int err;

	err = init_common( s, shader, x, y, w, h, c );
	if ( err ) {
		return err;
	}

	return mesh_init_quad( &s->mesh, UNIT_QUAD, DRAW_STATIC );
}

/*
 * shape2d_tri_create() - Create a 2D triangle
 * @s:      struct shape2d
 * @shader: struct shader
 * @x:      float
 * @y:      float
 * @w:      float
 * @h:      float
 * @c:      colour(RGB)
 */
int shape2d_tri_create( struct shape2d *s, struct shader *shader, float x,
                        float y, float w, float h, colour c )
{
	int err;

	err = init_common( s, shader, x, y, w, h, c );
	if ( err )
		return err;

	return mesh_init_quad( &s->mesh, UNIT_TRI, DRAW_STATIC );
}

/*
 * shape2d_move() - Move x, y coordinate
 * @s
 * @dx
 * @dy
 */
void shape2d_move( struct shape2d *s, float dx, float dy )
{
	s->pos.x += dx;
	s->pos.y += dy;
}

/*
 * shape2d_set_pos() - Set x, y coordinate
 * @s
 * @x
 * @y
 */
void shape2d_set_pos( struct shape2d *s, float x, float y )
{
	s->pos.x = x;
	s->pos.y = y;
}

/*
 * shape2d_set_colour() - Set uniform colour
 * @s: struct shape2d
 * @c: colour
 */
void shape2d_set_colour( struct shape2d *s, colour c )
{
	s->colour = colourf_make( c );
}

/*
 * shape2d_draw() - Draw mesh and update uniform properties
 * @s
 */
void shape2d_draw( const struct shape2d *s )
{
	shader_set_vec2( s->shader, "u_pos", s->pos );
	shader_set_vec2( s->shader, "u_scale", s->scale );
	shader_set_4f( s->shader, "u_colour", s->colour.raw );
	renderer_draw_mesh( &s->mesh, s->shader );
}

/*
 * shape2d_destroy() - Destroy mesh
 * @s
 */
void shape2d_destroy( struct shape2d *s )
{
	mesh_destroy( &s->mesh );
}
