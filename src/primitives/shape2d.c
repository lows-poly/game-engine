#include <stdio.h>
#include <math.h>
#include <errno.h>

#include "shape2d.h"
#include "renderer/renderer.h"
#include "mathx.h"
#include "log.h"

/* 
 * #ifndef DEFAULT_SHAPE_COLOUR
 * #define DEFAULT_SHAPE_COLOUR	CYAN
 * #endif
 */

static const float TWO_PI = 2.0f * (float)ENGINE_PI;

static colour DEFAULT_SHAPE_COLOUR = CYAN;

static float wrap_angle( float rad )
{
	rad = fmodf( rad, TWO_PI );

	if ( rad < 0.0f )
		rad += TWO_PI;

	return rad;
}

void shape2d_set_default_colour( colour c )
{
	DEFAULT_SHAPE_COLOUR = c;
}

bool shape2d_create( struct shape2d *s, enum shape2d_type type, float x, float y,
                     float w, float h )
{
	if ( !s )
		return false;
	
	switch ( type ) {
	case SHAPE2D_RECTANGLE:
		break;
	case SHAPE2D_TRIANGLE:
		break;
	default:
		pr_err("UNKNOWN SHAPE TYPE\n");
		return false;
	}

	s->type = type;
	s->colour = DEFAULT_SHAPE_COLOUR;
	s->pos = vec2_make( x, y );
	s->scale = vec2_make( w, h );
	s->origin = vec2_make( w * 0.5f, h * 0.5f );
	s->rotation = 0.0f;

	return true;
}

/*
 * shape2d_move() - Move x, y coordinate
 * @s
 * @dx
 * @dy
 */
void shape2d_move( struct shape2d *s, float dx, float dy )
{
	if ( !s )
		return;

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
	if ( !s )
		return;

	s->pos.x = x;
	s->pos.y = y;
}

void shape2d_set_rotation( struct shape2d *s, float rad )
{
	if ( !s )
		return;

	s->rotation = wrap_angle( s->rotation + rad );
}

void shape2d_rotate( struct shape2d *s, float rad )
{
	if ( !s )
		return;

	s->rotation += rad;
}

void shape2d_set_origin( struct shape2d *s, float x, float y )
{
	if ( !s )
		return;

	s->origin = vec2_make( x, y );
}

void shape2d_center_origin( struct shape2d *s )
{
	if ( !s )
		return;

	s->origin = vec2_make( s->scale.x * 0.5f, s->scale.y * 0.5f );
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
 * shape2d_destroy()
 * @s
 */
void shape2d_destroy( struct shape2d *s )
{
	(void)s;
}
