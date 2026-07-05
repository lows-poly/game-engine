/*
 * window.h - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 * Callers own a `struct window` and pass it by pointer to every function here.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>

struct window {
	GLFWwindow *handle;
	int width;
	int height;
	int resized;
};

int window_init( struct window *w );
int window_should_close( struct window *w );
void window_swap_and_poll( struct window *w );
void window_destroy( struct window *w );

#endif
