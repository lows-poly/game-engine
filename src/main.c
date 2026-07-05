#include <stdlib.h>

#include "window/window.h"

int main( void ) {
	struct window win;

	if ( !window_init( &win ) )
		return EXIT_FAILURE;

	while ( !window_should_close( &win ) ) {
		glClear( GL_COLOR_BUFFER_BIT );
		window_swap_and_poll( &win );
	}

	window_destroy( &win );

	return EXIT_SUCCESS;
}
