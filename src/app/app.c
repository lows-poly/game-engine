#include "audio/audio.h"
#include <string.h>

#include "app.h"
#include "log.h"
#include "path.h"

#define FLAGS_AUDIO ( 1 << 0 )
#define FLAGS_FONT ( 1 << 1 )

#define APP_DEFAULT_FONT_PATH "assets/fonts/Ac437_Acer710_Mono.ttf"
#define APP_DEFAULT_FONT_HEIGHT 16.0f

#define APP_FIXED_DT ( 1.0 / 60.0 )

bool app_init( struct app *app, int width, int height, const char *title,
               unsigned int init_flags )
{
	if ( !app )
		return false;

	memset( app, 0, sizeof( *app ) );

	if ( !path_root() && !path_init() )
		pr_warn(
			"RESOURCE ROOT NOT FOUND; RELATIVE ASSET PATHS REQUIRE CONFIGURATION\n" );

	if ( window_init( &app->win, width, height, title ) ) {
		pr_warn( "FAILED TO INIT WINDOW\n" );
		goto ERR;
	}

	app->window_init = true;

	if ( !input_init( &app->input, &app->win ) ) {
		pr_warn( "FAILED TO INIT INPUT\n" );
		goto ERR;
	}

	app->input_init = true;

	if ( !timer_init( &app->timer, APP_FIXED_DT ) ) {
		pr_warn( "FAILED TO INIT TIMER\n" );
		goto ERR;
	}

	if ( init_flags & FLAGS_AUDIO ) {
		if ( !audio_system_init( &app->audio ) ) {
			pr_warn( "FAILED TO INIT AUDIO SYSTEM" );
			goto ERR;
		}

		app->audio_init = true;
	}

	if ( init_flags & FLAGS_FONT ) {
		if ( !font_init( &app->default_font, APP_DEFAULT_FONT_PATH,
		                 APP_DEFAULT_FONT_HEIGHT, NULL ) )
		{
			pr_warn( "DEFAULT FONT UNAVAILABLE\n" );
			goto ERR;
		}

		app->font_init = true;
	}

	app->running = true;
	return true;

ERR:
	app_shutdown( app );
	return false;
}

bool app_has_audio( struct app *app )
{
	return app->audio_init;
}

bool app_has_font( struct app *app )
{
	return app->font_init;
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

	if ( !app->audio_init ) {
		if ( app->font_init )
			goto DESTROY_FONT;
		else
			goto DESTROY_INPUT;
	}

	audio_system_destroy( &app->audio );
	app->audio_init = false;

DESTROY_FONT:
	if ( !app->font_init )
		goto DESTROY_INPUT;

	font_destroy( &app->default_font );
	app->font_init = false;

DESTROY_INPUT:
	if ( !app->input_init )
		goto DESTROY_WINDOW;

	input_destroy( &app->input, &app->win );
	app->input_init = false;

DESTROY_WINDOW:
	if ( app->window_init ) {
		window_destroy( &app->win );
		app->window_init = false;
	}

	memset( app, 0, sizeof( *app ) );
}
