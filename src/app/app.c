/*
 * app_init() - Initialise window and input
 * @app:    app struct
 * @argv0
 * @width:  Window width
 * @height: Window height
 * @title:  Window title
 */

#include "app.h"
#include "path.h"
#include "log.h"

bool app_init( struct app *app, const char *argv0, int width, int height,
               const char *title )
{
	if ( path_init( argv0 ) < 0 ) {
		pr_warn("FAILED TO INIT PATH\n");
		return false;
	}

	if ( !window_init( &app->win, width, height, title ) ) {
		pr_warn("FAILED TO INIT WINDOW\n");
		return false;
	}

	input_init( &app->input, &app->win );

	if ( !timer_init( &app->time, 1.0 / 60.0 ) ) {
		pr_warn("FAILED TO INIT TIMER\n");
		return false;
	}
	
	app->running = true;
	return true;
}

/*
 * app_update() - Update input, swap buffers, poll events, and limit fps
 * @app: app struct
 */
void app_update( struct app *app )
{
	timer_tick( &app->time );

	input_update( &app->input );
	window_end_frame( &app->win );
	window_limit_fps( &app->win );
	app->running = !window_should_close( &app->win );
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
