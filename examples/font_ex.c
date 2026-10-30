#include <stdlib.h>

#include "app/app.h"
#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"

#include "graphics/font/font.h"

#define WINDOW_TITLE "FONT EXAMPLE"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main( int argc, char *argv[] )
{
	struct app app;
	struct renderer_2d renderer;
	
	struct font font_ac437_8x16;
	struct font font_ac437_Acer;
	struct font font_proggy;

	(void)argc;

	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	if ( !renderer_2d_init( &renderer, WINDOW_WIDTH, WINDOW_HEIGHT ) )
		goto SHUTDOWN_EXIT;

	if ( !renderer_2d_set_view( &renderer, &renderer.view ) )
		goto SHUTDOWN_EXIT;

	if ( !font_init( &font_ac437_8x16, "assets/fonts/Ac437_ACM_VGA_8x16.ttf", 16.0f, NULL ) )
		goto CLEAN_FONT;

	if ( !font_init( &font_ac437_Acer, "assets/fonts/Ac437_Acer710_Mono.ttf", 16.0f, NULL ) )
		goto CLEAN_FONT;

	if ( !font_init( &font_proggy, "assets/fonts/ProggyClean.ttf", 32.0f, NULL ) )
		goto CLEAN_FONT;

	while ( app_begin_frame( &app ) ) {
		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		renderer_begin_frame( CYAN );
		renderer_2d_update( &renderer, &app.win );

		renderer_2d_draw_text( &renderer, &font_ac437_8x16,
		"Ac437_ACM_VGA_8x16 rendering test\nLoading @ 16.0f pixel height",
		                       40.0f, 40.0f, 2.5f, WHITE );

		renderer_2d_draw_text( &renderer, &font_ac437_Acer,
		"Ac437_Acer710_Mono rendering test\nLoading @ 16.0f pixel height",
		                       40.0f, 150.0f, 2.5f, WHITE );

		renderer_2d_draw_text( &renderer, &font_proggy,
		"ProggyClean rendering test\nLoading @ 32.0f pixel height",
		                       40.0f, 265.0f, 1.0f, WHITE );

		renderer_2d_draw_text( &renderer, &font_ac437_8x16,
		"PRESS [ESCAPE] TO EXIT THE PROGRAM",
		                       44.0f, 374.0f, 3.5f, BLACK );
		renderer_2d_draw_text( &renderer, &font_ac437_8x16,
		"PRESS [ESCAPE] TO EXIT THE PROGRAM",
		                       40.0f, 370.0f, 3.5f, BLUE );

		app_end_frame( &app );
	}

	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;

CLEAN_FONT:
	font_destroy( &font_ac437_8x16 );
	font_destroy( &font_ac437_Acer );
	font_destroy( &font_proggy );

SHUTDOWN_EXIT:
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );
	return EXIT_FAILURE;
}
