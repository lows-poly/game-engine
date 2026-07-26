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

/*
 * app_init() - Initialise window and input
 * @app:    app struct
 * @width:  Window width
 * @height: Window height
 * @title:  Window title
 */
int app_init( struct app *app, int width, int height, const char *title );

/*
 * app_running() - Update input, swap buffers, poll events, and limit fps
 * @app: app struct
 */
int app_running( struct app *app );

/*
 * app_stop() - Set close window
 * @app: app struct
 */
void app_stop( struct app *app );

/*
 * app_shutdown() - Destroy input and window
 * @app: app struct
 */
void app_shutdown( struct app *app );

#endif
