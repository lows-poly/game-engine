#include "window/window.h"

#define WINDOW_TITLE         "Hello Window"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

int main( void )
{
	struct window win;

	if ( !window_init( &win, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE ) )
		return -1;

	while ( !window_should_close( &win ) ) {
		/* Your code here */
		 
		/* Swap buffers & poll events */
		window_end_frame( &win );
	}

	window_close( &win );

	return 0;
}
