#include <stdio.h>
#include <stdlib.h>

#include "app/app.h"
#include "window/window.h"
#include "input/input.h"

#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"

#include "primitives/shape2d.h"
#include "primitives/colour.h"

#include "log.h"

#define WINDOW_TITLE         "ENGINE"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

#define SPEED                200.0f

int main( int argc, char *argv[] )
{
	struct app app;
	struct shape2d rect;
	struct shape2d tri;
	struct renderer_2d renderer;
	float dt, velocity;

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	/* SHAPE SETUP */
	if ( !shape2d_create( &rect, SHAPE2D_RECTANGLE, 300.0f, 300.0f, 64.0f, 64.0f ) )
		return EXIT_FAILURE;

	if ( !shape2d_create( &tri, SHAPE2D_TRIANGLE, 600.0f, 100.0f, 64.0f, 64.0f ) )
		return EXIT_FAILURE;

	shape2d_set_colour( &tri, GREEN );

	/* RENDERER SETUP */
	if ( renderer_2d_init( &renderer, app.win.width, app.win.height ) != 0 )
		return EXIT_FAILURE;

	while ( app.running ) {
		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

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
			shape2d_move( &rect, 0.0f, velocity );

		if ( key_down( &app.input, KEY_W ) )
			shape2d_move( &rect, 0.0f, -velocity );

		shape2d_rotate( &tri, 1.0f * dt );

		renderer_2d_draw_shape( &renderer, &rect );
		renderer_2d_draw_shape( &renderer, &tri );

		app_update( &app );
	}

	shape2d_destroy( &rect );
	shape2d_destroy( &tri );
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
