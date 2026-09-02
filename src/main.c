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

#define WINDOW_TITLE         "ENGINE"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

#define MAX_STARS          256

int main( int argc, char *argv[] )
{
	struct app app;
	struct shape2d tri;
	struct renderer_2d renderer;

	struct shape2d stars[MAX_STARS];
	int start_count;
	int i;

	float dt;

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	/* RENDERER SETUP */
	if ( renderer_2d_init( &renderer, app.win.width, app.win.height ) != 0 )
		return EXIT_FAILURE;

	srand(time(NULL));
	shape2d_set_default_colour( WHITE );

	for ( i = 0; i < MAX_STARS; i++ ) {
		vec2 pos = { randf( 0.1f, (float)WINDOW_WIDTH ),
		             randf( 0.1f, (float)WINDOW_HEIGHT) };
		vec2 scale = { randf( 0.1f, 2.5f ), randf( 0.1f, 2.5f ) };
		shape2d_create( &stars[i], SHAPE2D_RECTANGLE, randf( 0.1f,
		                (float)WINDOW_WIDTH ), randf( 0.1f, (float)WINDOW_HEIGHT ),
		                randf( 1.0f, 2.5f ), randf( 1.0f, 2.5f ) );
	}

	while ( app.running ) {
		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		dt = (float)app.time.delta_time;

		
		for ( i = 0; i < MAX_STARS; i++ ) {
			renderer_2d_draw_shape( &renderer, &stars[i] );
		}

		app_update( &app );
	}

	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
