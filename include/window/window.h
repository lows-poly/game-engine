/*
 * window.h - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 * Callers own a `struct window` and pass it by pointer to every function here.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <glad.h>
#include <GLFW/glfw3.h>

struct window_state {
	GLFWwindow *handle;
	int width;
	int height;
	bool resized;
};

int window_init( struct window_state *w );
int window_should_close( struct window_state *w );
void window_set_should_close( struct window_state *w, bool value );
void window_poll_events( struct window_state *w );
void window_swap_buffers( struct window_state *w );
void window_destroy( struct window_state *w );

#endif
