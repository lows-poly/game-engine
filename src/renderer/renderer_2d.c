#include <errno.h>

#include "renderer_2d.h"
#include "renderer.h"
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
		return -EINVAL;

	if ( width <= 0 || height <= 0 ) {
		pr_warn("FAILED TO SET 2D PROJECTION: INVALID ARGUMENTS\n");
		return -EINVAL;
	}

	/*
	 * PIXEL COORDINATES:
	 * LEFT   = 0
	 * RIGHT  = WIDTH
	 * BOTTOM = HEIGHT
	 * TOP    = 0
	 */
	mat4_ortho( 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, proj );

	shader_use( r->shader );
	if ( !shader_set_mat4( r->shader, "u_projection", proj ) ) {
		pr_warn("FAILED TO SET 2D PROJECTION UNIFORM\n");
		return -EINVAL;
	}

	r->width = width;
	r->height = height;

	pr_info("SUCCESSFULLY SET 2D PROJECTION\n");
	return 0;
}

/*
 * renderer_2d_init() - Init shader 2d preset and set projection.
 * @renderer_2d struct
 * @width
 * @height
 *
 * return: 0 on success, failure otherwise.
 */
int renderer_2d_init( struct renderer_2d *r, int width, int height )
{
	if ( !r || width <= 0 || height <= 0 ) {
		if ( !r )
			pr_warn("RENDERER_2D IS NULL\n");
		else
			pr_warn("WIDTH, HEIGHT IS EXPECTED TO BE ABOVE ZERO\n");

		return -EINVAL;
	}

	if ( shader_init_preset( &r->default_shader, SHADER_PRIMITIVE_2D ) != 0 ) {
		pr_warn("FAILED TO INIT 2D RENDERER: ENCOUNTER ERROR FROM SHADER PRESET INIT\n");
		return -ENOMEM;
	}

	r->shader = &r->default_shader;
	r->width = width;
	r->height = height;

	mesh_init_quad( &r->rect_mesh, UNIT_QUAD, DRAW_STATIC );
	mesh_init_tri( &r->tri_mesh, UNIT_TRI, DRAW_STATIC );

	return set_projection( r, width, height );
}

int renderer_2d_set_shader( struct renderer_2d *r, struct shader *shader )
{
	struct shader *prev;
	int err;

	if ( !r || !shader )
		return -EINVAL;

	prev = r->shader;
	r->shader = shader;

	err = set_projection( r, r->width, r->height );
	if ( err != 0 ) {
		r->shader = prev;
		return err;
	}

	return 0;
}

void renderer_2d_use_default_shader( struct renderer_2d *r )
{
	if ( !r )
		return;

	r->shader = &r->default_shader;
}

void renderer_2d_update( struct renderer_2d *r, const struct window *w )
{
	if ( !r || !w || !r->shader )
		return;

	if ( !w->resized )
		return;

	set_projection( r, w->width, w->height );
}

void renderer_2d_draw_shape( struct renderer_2d *r, struct shape2d *shape )
{
	struct mesh *mesh;
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

	shader_use( r->shader );
	shader_set_vec2( r->shader, "u_pos", shape->pos );
	shader_set_vec2( r->shader, "u_scale", shape->scale );
	shader_set_4f( r->shader, "u_colour", colour_arr );
	renderer_draw_mesh( mesh, r->shader );
}

int renderer_2d_resize( struct renderer_2d *r, int width, int height )
{
	if ( width <= 0 || height <= 0 )
		return -EINVAL;

	return set_projection( r, width, height );
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
