#include <stdlib.h>

#include "window/window.h"
#include "input/input.h"
#include "timer/timer.h"

#include "renderer/renderer.h"

#include "primitives/colour.h"
#include "primitives/colourf.h"

#define WINDOW_TITLE		"ENGINE"
#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
#define TARGET_FRAMERATE	1.0 / 60.0

int main( void )
{
	struct window_state window;
	struct input_state input;
	struct timer tm;

	if ( !window_init( &window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE ) )
		return EXIT_FAILURE;

	input_init( &input, window.handle );
	timer_init( &tm, TARGET_FRAMERATE );

	while ( !window_should_close( &window ) ) {
		window_poll_events( &window );

		renderer_clear_colour( BLACK );
		renderer_clear( GL_COLOR_BUFFER_BIT );

		timer_tick( &tm );

		while ( timer_should_step( &tm ) ) {
			timer_step( &tm );
		}

		if ( input_key_pressed( &input, KEY_ESCAPE ) )
			window_set_should_close( &window, true );

		window_swap_buffers( &window );
		input_update( &input );
	}

	input_destroy( &input, window.handle );
	window_destroy( &window );

	return EXIT_SUCCESS;
}
