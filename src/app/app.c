#include <string.h>

#include "app.h"
#include "log.h"
#include "path.h"

#define APP_FIXED_DT	(1.0 / 60.0)

bool app_init( struct app *app, int width, int height, const char *title, const char *argv0 )
{
	if ( !app )
		return false;

	memset( app, 0, sizeof( *app ) );

	if ( argv0 && path_init( argv0 ) < 0 )
		pr_warn("FAILED TO INIT PATH\n");

	if ( !window_init( &app->win, width, height, title ) ) {
		pr_warn("FAILED TO INIT WINDOW\n");
		goto ERR;
	}

	app->window_init = true;

	if ( !input_init( &app->input, &app->win ) ) {
		pr_warn("FAILED TO INIT INPUT\n");
		goto ERR;
	}

	app->input_init = true;

	if ( !timer_init( &app->timer, APP_FIXED_DT ) ) {
		pr_warn("FAILED TO INIT TIMER\n");
		goto ERR;
	}

	if ( !audio_system_init( &app->audio ) ) {
		pr_warn("FAILED TO INIT AUDIO SYSTEM\n");
		goto ERR;
	}

	app->running = true;
	return true;

ERR:
	app_shutdown( app );
	return false;
}

bool app_begin_frame( struct app *app )
{
	if ( !app || !app->running || !app->window_init )
		return false;

	input_begin_frame( &app->input );
	window_poll_events();
	timer_tick( &app->timer );

	app->running = !window_should_close( &app->win );

	return app->running;
}

void app_end_frame( struct app *app )
{
	if ( !app || !app->window_init )
		return;

	window_swap_buffers( &app->win );
}

void app_stop( struct app *app )
{
	if ( !app )
		return;

	app->running = false;
	window_set_should_close( &app->win, true );
}

void app_shutdown( struct app *app )
{
	if ( !app )
		return;

	app->running = false;

	audio_system_destroy( &app->audio );

	if ( app->input_init ) {
		input_destroy( &app->input, &app->win );
		app->input_init = false;
	}

	if ( app->window_init ) {
		window_destroy( &app->win );
		app->window_init = false;
	}

	memset( app, 0, sizeof( *app ) );
}
