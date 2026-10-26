#include <stdlib.h>

#include "app/app.h"
#include "input/input.h"
#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"
#include "graphics/texture/texture.h"
#include "primitives/shape2d.h"
#include "primitives/colour.h"

#define WINDOW_TITLE "TEXTURED RECTANGLE"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RECTANGLE_WIDTH 256.0f
#define RECTANGLE_HEIGHT 256.0f

int main( int argc, char *argv[] )
{
	struct app app;
	struct shape2d rect;
	struct renderer_2d renderer;
	struct texture texture;
	struct texture_config texture_config = TEXTURE_CONFIG_DEFAULT;

	(void)argc;

	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	if ( !shape2d_create( &rect, SHAPE2D_RECTANGLE, 272.0f, 172.0f,
	                      RECTANGLE_WIDTH, RECTANGLE_HEIGHT ) )
	{
		app_shutdown( &app );
		return EXIT_FAILURE;
	}

	if ( !renderer_2d_init( &renderer, WINDOW_WIDTH, WINDOW_HEIGHT ) ) {
		shape2d_destroy( &rect );
		app_shutdown( &app );
		return EXIT_FAILURE;
	}

	if ( !texture_init( &texture ) )
		return EXIT_FAILURE;

	if ( !texture_load( &texture, "assets/textures/dummy.png", 0,
	                    &texture_config ) )
	{
		renderer_2d_destroy( &renderer );
		shape2d_destroy( &rect );
		app_shutdown( &app );
		return EXIT_FAILURE;
	}

	while ( app_begin_frame( &app ) ) {
		renderer_2d_update( &renderer, &app.win );
		renderer_begin_frame( CYAN );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		renderer_2d_draw_texture( &renderer, &rect, &texture, WHITE );

		app_end_frame( &app );
	}

	texture_destroy( &texture );
	renderer_2d_destroy( &renderer );
	shape2d_destroy( &rect );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
