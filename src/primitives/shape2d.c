#include <stdio.h>
#include <errno.h>

#include "shape2d.h"
#include "renderer/renderer.h"

/* 
 * #ifndef DEFAULT_SHAPE_COLOUR
 * #define DEFAULT_SHAPE_COLOUR	CYAN
 * #endif
 */

static colour DEFAULT_SHAPE_COLOUR = CYAN;

static const vec2 UNIT_QUAD[4] = {
	{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
};

static const vec2 UNIT_TRI[3] = {
	{ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f }
};

static void create_shape( struct shape2d *s, float x, float y, float w, float h )
{
	s->pos = vec2_make( x, y );
	s->scale = vec2_make( w, h );
	s->colour = DEFAULT_SHAPE_COLOUR;
}

void shape2d_set_default_colour( colour c )
{
	DEFAULT_SHAPE_COLOUR = c;
}

bool shape2d_create_rect( struct shape2d *s, float x, float y, float w, float h )
{
	if ( !s || x < 0 || y < 0 || w < 0 || h < 0 )
		return false;
	
	create_shape( s, x, y, w, h );

	return mesh_init_quad( &s->mesh, UNIT_QUAD, DRAW_STATIC ) == 0;
}

bool shape2d_create_tri( struct shape2d *s, float x, float y, float w, float h )
{
	if ( !s || x < 0 || y < 0 || w < 0 || h < 0 )
		return false;
	
	create_shape( s, x, y, w, h );

	return mesh_init_tri( &s->mesh, UNIT_TRI, DRAW_STATIC ) == 0;
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
	if ( !s )
		return;

	s->colour = c;
}

/*
 * shape2d_destroy() - Destroy mesh
 * @s
 */
void shape2d_destroy( struct shape2d *s )
{
	mesh_destroy( &s->mesh );
}
