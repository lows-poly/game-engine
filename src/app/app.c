#include "app.h"

/*
 * app_init() - Initialise window and input
 * @app:    app struct
 * @width:  Window width
 * @height: Window height
 * @title:  Window title
 */
int app_init( struct app *app, int width, int height, const char *title )
{
	if ( !window_init( &app->win, width, height, title ) )
		return 0;

	input_init( &app->input, &app->win );

	return 1;
}

/*
 * app_running() - Update input, swap buffers, poll events, and limit fps
 * @app: app struct
 */
int app_running( struct app *app )
{
	input_update( &app->input );
	window_end_frame( &app->win );
	window_limit_fps( &app->win );

	return !window_should_close( &app->win );
}

/*
 * app_stop() - Set close window
 * @app: app struct
 */
void app_stop( struct app *app )
{
	window_set_should_close( &app->win, 1 );
}

/*
 * app_shutdown() - Destroy input and window
 * @app: app struct
 */
void app_shutdown( struct app *app )
{
	input_destroy( &app->input, &app->win );
	window_close( &app->win );
}
