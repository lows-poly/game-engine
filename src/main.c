#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app/app.h"
#include "window/window.h"
#include "input/input.h"

#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"

#include "primitives/shape2d.h"
#include "primitives/colour.h"

#include "mathx.h"
#include "log.h"

#define WINDOW_TITLE	"ENGINE"
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

#define SHAPE_SCALE	100.0f

int main( int argc, char *argv[] )
{
	struct app app;
	struct renderer_2d renderer;
	
	struct shape2d rect;

	float dt;

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	/* RENDERER SETUP */
	if ( renderer_2d_init( &renderer, WINDOW_WIDTH, WINDOW_HEIGHT ) != 0 )
		return EXIT_FAILURE;

	if ( !shape2d_create( &rect, SHAPE2D_RECTANGLE, (float)WINDOW_WIDTH / 2,
	                     (float)WINDOW_HEIGHT / 2, SHAPE_SCALE, SHAPE_SCALE ) )
		return EXIT_FAILURE;

	while ( app.running ) {
		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		dt = (float)app.time.delta_time;

		renderer_2d_draw_shape( &renderer, &rect );
		
		app_update( &app );
		renderer_end_frame( &app.win );
	}

	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
