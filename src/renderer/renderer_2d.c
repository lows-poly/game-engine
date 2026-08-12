#include <errno.h>

#include "renderer_2d.h"
#include "renderer.h"
#include "log.h"

static int set_projection( struct renderer_2d *r, int width, int height )
{
	mat4 proj;

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

	return set_projection( r, width, height );
}

/* TODO */
void renderer_2d_set_shader( struct renderer_2d *r, struct shader *shader )
{
	if ( !r || !shader )
		return;

	r->shader = shader;
}

void renderer_2d_update( struct renderer_2d *r, const struct window *w )
{
	if ( !w->resized )
		return;

	set_projection( r, w->width, w->height );
}

void renderer_2d_draw( struct renderer_2d *r, struct shape2d *shape )
{
	float colour_arr[4];
	colour_to_arr( shape->colour, colour_arr );

	shader_use( &r->shader );
	shader_set_vec2( r->shader, "u_pos", shape->pos );
	shader_set_vec2( r->shader, "u_scale", shape->scale );
	shader_set_4f( r->shader, "u_colour", colour_arr );
	renderer_draw_mesh( &shape->mesh, r->shader );
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

	r->shader = NULL;
	r->width = 0;
	r->height = 0;
}
