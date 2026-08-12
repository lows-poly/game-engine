#include "app/app.h"

#define WINDOW_TITLE         "Hello Window"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

int main( void )
{
	struct app application;

	if ( !app_init( &application, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL ) )
		return -1;

	while ( application.running ) {
		/* Your code here */

		app_update( &application );
	}

	app_shutdown( &application );

	return 0;
}
