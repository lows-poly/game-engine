#ifndef APP_H
#define APP_H

#include "window/window.h"
#include "input/input.h"
#include "renderer/renderer.h"

/*
 * struct app - Application
 * @win:   window struct
 * @input: input_state struct
 */
struct app {
	struct window win;
	struct input_state input;
};

int app_init( struct app *app, int width, int height, const char *title );
int app_running( struct app *app );
void app_stop( struct app *app );
void app_shutdown( struct app *app );

#endif
