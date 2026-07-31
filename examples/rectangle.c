#include <stdio.h>

#include "path.h"

#include "app/app.h"
#include "window/window.h"
#include "input/input.h"

#include "renderer/renderer.h"

#include "graphics/mesh.h"
#include "graphics/shader/shader.h"

#include "primitives/shape2d.h"
#include "primitives/colour.h"

#define WINDOW_TITLE         "Rectangle Test"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

#define SPEED                0.2f

int main( int argc, char *argv[] )
{
	struct app app;
	struct shader shader;
	struct shape2d rect;
	struct shape2d tri;
	float dt;
	float velocity;

	/* PATH SETUP */
	if ( path_init( argv[0] ) < 0 )
		return -1;

	/* WINDOW SETUP */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE ) )
		return -1;

	window_set_vsync( &app.win, 1 );

	/* SHADER SETUP */
	if ( shader_init_preset( &shader, SHADER_PRIMITIVE_2D ) != 0 )
		return -1;

	/* SHAPE SETUP */
	if ( shape2d_rect_create( &rect, &shader, 0.0f, 0.0f, 0.5, 0.5f, CYAN ) != 0 )
		return -1;
	if ( shape2d_tri_create( &tri, &shader, 0.0f, 0.0f, 0.5, 0.5f, RED ) != 0 )
		return -1;

	while ( app.running ) {
		renderer_begin_frame( VINTAGE_GOLD );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		dt = (float)app.time.delta_time;
		velocity = SPEED * dt;

		/* RECT CONTROLS */
		if ( key_down( &app.input, KEY_A ) )
			shape2d_move( &rect, -velocity, 0.0f );

		if ( key_down( &app.input, KEY_D ) )
			shape2d_move( &rect, velocity, 0.0f );

		if ( key_down( &app.input, KEY_S ) )
			shape2d_move( &rect, 0.0f, -velocity );

		if ( key_down( &app.input, KEY_W ) )
			shape2d_move( &rect, 0.0f, velocity );

		/* TRIANGLE CONTROLS */
		if ( key_down( &app.input, KEY_LEFT ) )
			shape2d_move( &tri, -velocity, 0.0f );

		if ( key_down( &app.input, KEY_RIGHT ) )
			shape2d_move( &tri, velocity, 0.0f );

		if ( key_down( &app.input, KEY_DOWN ) )
			shape2d_move( &tri, 0.0f, -velocity );

		if ( key_down( &app.input, KEY_UP ) )
			shape2d_move( &tri, 0.0f, velocity );

		shape2d_draw( &rect );
		shape2d_draw( &tri );

		app_update( &app );
	}

	shape2d_destroy( &rect );
	shape2d_destroy( &tri );
	shader_destroy( &shader );
	app_shutdown( &app );

	return 0;
}
