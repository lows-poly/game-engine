#include <stdio.h>
#include <errno.h>

#include "shape2d.h"
#include "renderer/renderer.h"
#include "log.h"

/* 
 * #ifndef DEFAULT_SHAPE_COLOUR
 * #define DEFAULT_SHAPE_COLOUR	CYAN
 * #endif
 */

static colour DEFAULT_SHAPE_COLOUR = CYAN;


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
