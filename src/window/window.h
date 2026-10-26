/**
 * window.h - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 */

#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <stdbool.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

struct input_state;

/**
 * struct window - Private/internal use window state
 * @handle:            GLFW window handle
 * @framebuffer_width
 * @framebuffer_height
 * @resized
 * @vsync
 * @glfw_init
 */
struct window {
	struct input_state *input;
	GLFWwindow *handle;
	int framebuffer_width;
	int framebuffer_height;
	bool resized;
	bool vsync;
	bool glfw_init;
};

/**
 * window_init() - Initialise GLFW and create window
 * @window
 * @width
 * @height
 * @title
 *
 * Initialise glfw, create the window, setup framebuffer callback, create the
 * OpenGL context, and load OpenGL functions.
 *
 * Return: True on success.
 */
bool window_init( struct window *w, int width, int height, const char *title );
void window_destroy( struct window *w );

void window_poll_events( void );
void window_swap_buffers( const struct window *w );

bool window_should_close( const struct window *w );
void window_set_should_close( struct window *w, bool should_close );

void window_set_vsync( struct window *w, bool enabled );

bool window_take_resize( struct window *w, int *width, int *height );

#endif
