#ifndef ENGINE_APP_H
#define ENGINE_APP_H

#include <stdbool.h>

#include "input/input.h"
#include "timer/timer.h"
#include "window/window.h"

/**
 * struct app
 * @window
 * @input
 * @timer
 * @running
 * @window_init
 * @input_init
 */
struct app {
	struct window win;
	struct input_state input;
	struct timer timer;
	bool running;
	bool window_init;
	bool input_init;
};

/*
 * app_init() - Initialise window, input, timer, and path(if any).
 * @app
 * @width
 * @height
 * @title
 * @argv0
 */
bool app_init( struct app *app, int width, int height, const char *title,
               const char *argv0 );
bool app_begin_frame( struct app *app );
void app_end_frame( struct app *app );
void app_stop( struct app *app );
void app_shutdown( struct app *app );

#endif
