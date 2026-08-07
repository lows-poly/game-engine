#include <stdio.h>
#include <errno.h>

#include "shape2d.h"
#include "renderer/renderer.h"

#include "debugf.h"

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

void shape2d_set_default_colour( colour c )
{
	DEFAULT_SHAPE_COLOUR = c;
}

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
int shape2d_create( const char *name, struct shape2d *s, struct shader *shader,
                    float x, float y, float w, float h )
{
	if ( !s || !shader )
		return 0;

	s->name = name;
	s->shader = shader;
	s->pos = vec2_make( x, y );
	s->scale = vec2_make( w, h );
	s->colour = DEFAULT_SHAPE_COLOUR;

	return 1;
}

/*
 * shape2d_init() - Initialise a created shaoe struct.
 * @shape: shape_type enum
 * @s:     struct shape2d
 *
 * Return:
 * 1 - Succes
 * 0 - Failure
 */
int shape2d_init( enum shape_type shape, struct shape2d *s )
{
	if ( !s )
		return 0;
	
	switch ( shape ) {
	case SHAPE2D_RECTANGLE:
		return mesh_init_quad( &s->mesh, UNIT_QUAD, DRAW_STATIC ) == 0;
	case SHAPE2D_TRIANGLE:
		return mesh_init_tri( &s->mesh, UNIT_TRI, DRAW_STATIC ) == 0;
	default:
		return 0;
	}
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
 * shape2d_draw() - Draw mesh and update uniform properties
 * @s
 */
void shape2d_draw( const struct shape2d *s )
{
	float colour_arr[4];
	colour_to_arr( s->colour, colour_arr );

	shader_set_vec2( s->shader, "u_pos", s->pos );
	shader_set_vec2( s->shader, "u_scale", s->scale );
	shader_set_4f( s->shader, "u_colour", colour_arr );
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

static const struct field_desc shape2d_fields[] = {
	FIELD_NESTED( struct shape2d, mesh, mesh_fields, MESH_FIELD_COUNT ),
	FIELD_NESTED( struct shape2d, colour, colour_fields, COLOUR_FIELD_COUNT ),
	FIELD_NESTED_PTR( struct shape2d, shader, shader_fields, SHADER_FIELD_COUNT ),
	FIELD_NESTED( struct shape2d, pos, vec2_fields, VEC2_FIELD_COUNT ),
	FIELD_NESTED( struct shape2d, scale, vec2_fields, VEC2_FIELD_COUNT )
};

#define SHAPE2D_FIELD_COUNT	( sizeof( shape2d_fields ) / sizeof( shape2d_fields[0] ) )

void shape2d_dump( const struct shape2d *s )
{
	if ( !s )
		return;

	printf( "%s {\n", s->name );
	dump( s, shape2d_fields, SHAPE2D_FIELD_COUNT, 1 );
	printf("}\n");
}
