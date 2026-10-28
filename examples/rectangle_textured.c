#include <stdlib.h>

#include "app/app.h"
#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"
#include "primitives/shape2d.h"

#define WINDOW_TITLE "TEXTURED RECTANGLE"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define RECTANGLE_WIDTH 256.0f
#define RECTANGLE_HEIGHT 256.0f

int main( int argc, char *argv[] )
{
	struct app app;
	struct renderer_2d renderer;
	struct view_2d view;
	struct texture texture;
	struct shape2d rect;

	(void)argc;

	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	if ( !renderer_2d_init( &renderer, WINDOW_WIDTH, WINDOW_HEIGHT ) )
		goto SHUTDOWN_EXIT;

	if ( !shape2d_create( &rect, SHAPE2D_RECTANGLE, WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f,
	                      RECTANGLE_WIDTH, RECTANGLE_HEIGHT ) )
	{
		goto SHUTDOWN_EXIT;
	}

	view = renderer.view;
	view.scale = VIEW_2D_FIT;
	view.anchor = VIEW_2D_CENTER;

	view.camera.pos = vec2_make(
		(float)WINDOW_WIDTH * 0.5f,
		(float)WINDOW_HEIGHT * 0.5f
	);

	if ( !renderer_2d_set_view( &renderer, &view ) )
		goto SHUTDOWN_EXIT;

	if ( !texture_init( &texture ) )
		goto CLEAN_EXIT;

	if ( !texture_load( &texture, "assets/textures/dummy.png", 0, NULL ) )
		goto CLEAN_EXIT;

	while ( app_begin_frame( &app ) ) {
		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		renderer_begin_frame( CYAN );
		renderer_2d_update( &renderer, &app.win );
		renderer_2d_draw_texture( &renderer, &rect, &texture, WHITE );

		app_end_frame( &app );
	}

	texture_destroy( &texture );
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;

CLEAN_EXIT:
	texture_destroy( &texture );

SHUTDOWN_EXIT:
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );
	return EXIT_FAILURE;
}
