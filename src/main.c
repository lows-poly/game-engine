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
	struct renderer_2d renderer;
	float dt, velocity;

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, argv[0], WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE ) )
		return EXIT_FAILURE;

	/* SHAPE SETUP */
	if ( !shape2d_create( &rect, 100.0f, 100.0f, 64.0f, 64.0f ) )
		return EXIT_FAILURE;

	if ( !shape2d_init( &rect, SHAPE2D_RECTANGLE ) )
		return EXIT_FAILURE;

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

		/* printf( "POS: (%.2f, %.2f)\n", rect.pos.x, rect.pos.y );
		printf( "SCALE: (%.2f, %.2f)\n", rect.scale.x, rect.scale.y ); */

		renderer_2d_draw( &renderer, &rect );

		app_update( &app );
	}

	shape2d_destroy( &rect );
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
