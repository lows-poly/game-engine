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

#define WINDOW_TITLE         "STARS"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

#define MAX_STARS            256

int main( int argc, char *argv[] )
{
	struct app app;
	struct shape2d tri;
	struct renderer_2d renderer;

	struct shape2d stars[MAX_STARS];
	vec2 pos;
	float scale;

	int i;

	float time_, dt;

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	/* RENDERER SETUP */
	if ( renderer_2d_init( &renderer, app.win.width, app.win.height ) != 0 )
		return EXIT_FAILURE;

	srand(time(0));
	shape2d_set_default_colour( WHITE );

	for ( i = 0; i < MAX_STARS; i++ ) {
		pos.x = randf( 0.1f, (float)WINDOW_WIDTH );
		pos.y = randf( 0.1f, (float)WINDOW_HEIGHT);
		
		scale = randf( 0.5f, 1.5f );

		shape2d_create( &stars[i], SHAPE2D_RECTANGLE, pos.x, pos.y,
		                scale, scale );
	}

	time_ = 0;

	while ( app.running ) {
		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		dt = (float)app.time.delta_time;
		time_ += dt;
		
		for ( i = 0; i < MAX_STARS; i++ ) {
			renderer_2d_draw_shape( &renderer, &stars[i] );

			if ( time_ >= 1.0f ) {
				time_ = 0.0f;
				shape2d_move( &stars[i], randf( -45.0f * dt, 45.0f * dt ),
			                      randf( -45.0f * dt, 45.0f * dt ) );
			}
		}

		app_update( &app );
	}

	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
