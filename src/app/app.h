#ifndef APP_H
#define APP_H

#include <stdbool.h>

#include "window/window.h"
#include "input/input.h"
#include "timer/timer.h"
#include "renderer/renderer.h"

/*
 * struct app - Application
 * @win:     window struct
 * @input:   input_state struct
 * @running: !glfwWindowShouldClose
 */
struct app {
	struct window win;
	struct input_state input;
	struct timer time;
	bool running;
};

/*
 * app_init() - Initialise window and input
 * @app:    app struct
 * @width:  Window width
 * @height: Window height
 * @title:  Window title
 */
bool app_init( struct app *app, int width, int height, const char *title );

/*
 * app_update() - Update input, swap buffers, poll events, and limit fps
 * @app: app struct
 */
void app_update( struct app *app );

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
