#include <stdlib.h>
#include <stdio.h>

#include "window/window.h"
#include "input/input.h"
#include "timer/timer.h"

#include "primitives/colour.h"
#include "primitives/colourf.h"

int main( void ) {
	struct window_state window;
	struct input_state input;
	struct timer tm;

	colourf vt_gold = colour_to_float( VINTAGE_GOLD );

	if ( !window_init( &window, 740, 480, "ENGINE" ) )
		return EXIT_FAILURE;

	input_init( &input, window.handle );
	timer_init( &tm, 1.0 / 60.0 );

	while ( !window_should_close( &window ) ) {
		window_poll_events( &window );

		glClearColor( vt_gold.r, vt_gold.g, vt_gold.b, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

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
