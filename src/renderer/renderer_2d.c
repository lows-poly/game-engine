#include <string.h>

#include "renderer_2d.h"
#include "renderer.h"
#include "primitives/mat4.h"
#include "log.h"

static const vec2 UNIT_QUAD[4] = {
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f }
};

static const vec2 UNIT_TRI[3] = {
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 0.5f, 0.0f }
};

static int set_projection( struct renderer_2d *r, int width, int height )
{
	mat4 proj;

	if ( !r || !r->shader )
		return 0;

	if ( width <= 0 || height <= 0 ) {
		pr_warn("FAILED TO SET 2D PROJECTION: INVALID ARGUMENTS\n");
		return 0;
	}

	/*
	 * PIXEL COORDINATES:
	 * LEFT   = 0
	 * RIGHT  = WIDTH
	 * TOP    = 0
	 * BOTTOM = HEIGHT
	 */
	mat4_ortho( 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, proj );

	shader_use( r->shader );

	if ( !shader_set_mat4( r->shader, "u_projection", proj ) ) {
		pr_warn("FAILED TO SET 2D PROJECTION UNIFORM\n");
		return 0;
	}

	r->width = width;
	r->height = height;

	return 1;
}

void renderer_2d_destroy( struct renderer_2d *r )
{
	if ( !r )
		return;

	shader_destroy( &r->default_shader );
	mesh_destroy( &r->rect_mesh );
	mesh_destroy( &r->tri_mesh );

	r->shader = NULL;
	r->width = 0;
	r->height = 0;
}

/*
 * renderer_2d_init() - Init shader 2d preset and set projection.
 * @renderer_2d struct
 * @width
 * @height
 *
 * return: false on failure.
 */
bool renderer_2d_init( struct renderer_2d *r, int width, int height )
{
	if ( !r || width <= 0 || height <= 0 ) {
		pr_warn("INVALID ARGUMENTS\n");
		return false;
	}

	memset( r, 0, sizeof( *r ) );

	if ( !mesh_init_quad( &r->rect_mesh, UNIT_QUAD, DRAW_STATIC ) ) {
		pr_warn("FAILED TO INIT MESH QUAD\n");
		return false;
	}

	if ( !mesh_init_tri( &r->tri_mesh, UNIT_TRI, DRAW_STATIC ) ) {
		pr_warn("FAILED TO INIT MESH TRI\n");
		mesh_destroy( &r->rect_mesh );
		return false;
	}

	if ( shader_init_preset( &r->default_shader, SHADER_BASIC_2D ) != 0 ) {
		pr_warn("FAILED TO INIT SHADER PRESET\n");
		mesh_destroy( &r->rect_mesh );
		mesh_destroy( &r->tri_mesh );
		return false;
	}

	r->shader = &r->default_shader;
	r->width = width;
	r->height = height;

	if ( !set_projection( r, width, height ) ) {
		pr_warn("PROJECTION FAILED\n");
		renderer_2d_destroy( r );
		return false;
	}

	return true;
}

void renderer_2d_update( struct renderer_2d *r, struct window *w )
{
	int width;
	int height;

	if ( !r || !w )
		return;

	if ( !window_take_resize( w, &width, &height ) )
		return;

	if ( width <= 0 || height <= 0 )
		return;

	if ( set_projection( r, width, height ) ) {
		r->width = width;
		r->height = height;
	}
}

bool renderer_2d_set_shader( struct renderer_2d *r, struct shader *shader )
{
	struct shader *prev;

	if ( !r || !shader )
		return false;

	prev = r->shader;
	r->shader = shader;

	if ( !set_projection( r, r->width, r->height ) ) {
		r->shader = prev;
		return false;
	}

	return true;
}

void renderer_2d_use_default_shader( struct renderer_2d *r )
{
	if ( !r )
		return;

	r->shader = &r->default_shader;
	if ( !set_projection( r, r->width, r->height ) )
		pr_warn("2D PROJECTION ENCOUNTERED AN ERROR WHILE SWITCHING TO DEFAULT SHADER\n");
}

void renderer_2d_draw_shape( struct renderer_2d *r, struct shape2d *shape )
{
	struct mesh *mesh;
	mat4 model;
	float colour_arr[4];

	if ( !r || !shape || !r->shader )
		return;

	switch ( shape->type ) {
	case SHAPE2D_RECTANGLE:
		mesh = &r->rect_mesh;
		break;
	case SHAPE2D_TRIANGLE:
		mesh = &r->tri_mesh;
		break;
	default:
		return;
	}

	colour_to_arr( shape->colour, colour_arr );

	mat4_identity( model );
	mat4_translate( model, shape->pos.x, shape->pos.y, 0.0f );
	mat4_rotate_z( model, shape->rotation );
	mat4_translate( model, -shape->origin.x, -shape->origin.y, 0.0f );
	mat4_scale_3f( model, shape->scale.x, shape->scale.y, 1.0f );

	shader_use( r->shader );
	if ( !shader_set_mat4( r->shader, "u_model", model ) ||
	     !shader_set_4f( r->shader, "u_colour", colour_arr ) )
		return;

	renderer_draw_mesh( mesh, r->shader );
}
