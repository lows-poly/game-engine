#include <stdio.h>

#include "path.h"

#include "app/app.h"
#include "window/window.h"
#include "input/input.h"

#include "renderer/renderer.h"

#include "graphics/mesh.h"
#include "graphics/shader/shader.h"

#include "primitives/colour.h"
#include "primitives/colourf.h"
#include "primitives/rect.h"

#define WINDOW_TITLE		"Rectangle Test"
#define WINDOW_WIDTH		900
#define WINDOW_HEIGHT		600

#define RECT_SPEED		0.2f

static const unsigned int RECT_INDICES[] = { 0, 1, 2, 2, 3, 0 };

static const struct vertex_attrib RECT_ATTRIBS[1] = {
	{
		.index      = 0,
		.size       = 2,
		.type       = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride     = sizeof( vec2 ),
		.offset     = 0,
	}
};

int main( int argc, char *argv[] )
{
	struct app app;
	struct shader shader;
	struct mesh rect_mesh;
	struct mesh_desc desc;
	colourf rect_colour;
	rect rect_bounds;
	vec2 rect_verts[4];
	
	float dt;

	/* PATH SETUP */
	if ( path_init( argv[0] ) < 0 )
		return -1;

	/* WINDOW SETUP */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE ) )
		return -1;

	window_set_vsync( &app.win, 1 );
	/* window_set_target_fps( &app.win, 10 ); */

	/* SHADER SETUP */
	if ( shader_binit( &shader, SHADER_UCOLOUR ) != 0 )
		return -1;

	/* RECT SETUP */
	rect_colour = colourf_make( colour_make( 0, 150, 150 ) );
	rect_bounds = rect_make_vec2( vec2_make( 0.0f, 0.0f ), vec2_make( 0.5f, 0.5f ) );
	rect_to_vertices( &rect_bounds, rect_verts );

	/* MESH SETUP */
	desc.vertices = rect_verts;
	desc.vertex_size = sizeof( rect_verts );
	desc.attribs = RECT_ATTRIBS;
	desc.attrib_count = 1;
	desc.indices = RECT_INDICES;
	desc.vertex_count = 4;
	desc.index_count = 6;

	if ( mesh_init( &rect_mesh, &desc, DRAW_DYNAMIC ) != 0 )
		return -1;

	while ( app.running ) {
		renderer_begin_frame( VINTAGE_GOLD );

		if ( key_pressed( &app.input, KEY_ESCAPE ) ) {
			app_stop( &app );
		}

		dt = (float)app.time.delta_time;

		if ( key_down( &app.input, KEY_A ) ) {
			rect_bounds.x -= RECT_SPEED * dt;
			printf( "POS: %.1f, %.1f\n", rect_bounds.x, rect_bounds.y );
		}
		if ( key_down( &app.input, KEY_D ) ) {
			rect_bounds.x += RECT_SPEED * dt;
			printf( "POS: %.1f, %.1f\n", rect_bounds.x, rect_bounds.y );
		}
		if ( key_down( &app.input, KEY_S ) ) {
			rect_bounds.y -= RECT_SPEED * dt;
			printf( "POS: %.1f, %.1f\n", rect_bounds.x, rect_bounds.y );
		}
		if ( key_down( &app.input, KEY_W ) ) {
			rect_bounds.y += RECT_SPEED * dt;
			printf( "POS: %.1f, %.1f\n", rect_bounds.x, rect_bounds.y );
		}

		rect_to_vertices( &rect_bounds, rect_verts );
		mesh_update_vertices( &rect_mesh, rect_verts, sizeof( rect_verts ), 0 );

		renderer_draw_mesh( &shader, &rect_mesh );

		shader_set_4f( &shader, "vert_colour", rect_colour.raw );
		app_update( &app );
	}

	mesh_destroy( &rect_mesh );
	shader_destroy( &shader );
	app_shutdown( &app );

	return 0;
}
