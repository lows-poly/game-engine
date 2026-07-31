#include <errno.h>

#include "shape2d.h"
#include "renderer/renderer.h"

static const vec2 UNIT_QUAD[4] = {
	{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
};

static const vec2 UNIT_TRI[3] = {
	{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f }
};

static int shape_init_common( struct shape2d *s, struct shader *shader, float x,
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

int shape2d_rect_create( struct shape2d *s, struct shader *shader, float x,
                         float y, float w, float h, colour c )
{
	int err;

	err = shape_init_common( s, shader, x, y, w, h, c );
	if ( err ) {
		return err;
	}

	return mesh_init_quad( &s->mesh, UNIT_QUAD, DRAW_STATIC );
}

int shape2d_tri_create( struct shape2d *s, struct shader *shader, float x,
                         float y, float w, float h, colour c )
{
	int err;

	err = shape_init_common( s, shader, x, y, w, h, c );
	if ( err )
		return err;

	return mesh_init_quad( &s->mesh, UNIT_TRI, DRAW_STATIC );
}

void shape_move( struct shape2d *s, float dx, float dy )
{
	s->pos.x += dx;
	s->pos.y += dy;
}

void shape_set_pos( struct shape2d *s, float x, float y )
{
	s->pos.x = x;
	s->pos.y = y;
}

void shape_set_colour( struct shape2d *s, colour c )
{
	s->colour = colourf_make( c );
}

void shape_draw( const struct shape2d *s )
{
	shader_set_vec2( s->shader, "u_pos", s->pos );
	shader_set_vec2( s->shader, "u_scale", s->scale );
	shader_set_4f( s->shader, "u_colour", s->colour.raw );
	renderer_draw_mesh( &s->mesh, s->shader );
}

void shape_destroy( struct shape2d *s )
{
	mesh_destroy( &s->mesh );
}
