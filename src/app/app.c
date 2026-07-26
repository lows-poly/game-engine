#include "app.h"

int app_init( struct app *app, int width, int height, const char *title )
{
	if ( !window_init( &app->win, width, height, title ) )
		return 0;

	input_init( &app->input, &app->win );

	return 1;
}

int app_running( struct app *app )
{
	input_update( &app->input );
	window_end_frame( &app->win );
	window_limit_fps( &app->win );

	return !window_should_close( &app->win );
}

void app_stop( struct app *app )
{
	window_set_should_close( &app->win, 1 );
}

void app_shutdown( struct app *app )
{
	input_destroy( &app->input, &app->win );
	window_close( &app->win );
}
