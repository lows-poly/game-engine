#include <stdlib.h>

#include "window/window.h"
#include "input/input.h"

int main( void ) {
	struct window_state window;
	struct input_state input;

	if ( !window_init( &window, 740, 480, "ENGINE" ) )
		return EXIT_FAILURE;

	input_init( &input, window.handle );

	while ( !window_should_close( &window ) ) {
		window_poll_events( &window );
		glClear( GL_COLOR_BUFFER_BIT );

		if ( input_key_pressed( &input, KEY_ESCAPE ) )
			window_set_should_close( &window, true );

		window_swap_buffers( &window );
		input_update( &input );
	}

	input_destroy( &input, &window );
	window_destroy( &window );

	return EXIT_SUCCESS;
}
