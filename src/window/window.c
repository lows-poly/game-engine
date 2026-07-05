/*
 * window.c - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 * Callers own a `struct window` and pass it by pointer to every function here.
 */

#include <stdio.h>

#include "window/window.h"
#include "config.h"

static void framebuffer_size_callback( GLFWwindow *handle, int width, int height ) {
	struct window *w = glfwGetWindowUserPointer( handle );

	w->width = width;
	w->height = height;
	w->resized = 1;

	glViewport( 0, 0, width, height );
}

int window_init( struct window *w ) {
	if ( !glfwInit() ) {
		fprintf(stderr, "FAILED TO INIT GLFW\n");
		return 0;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // macos

	w->handle = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
	                              NULL, NULL );

	if ( !w->handle ) {
		fprintf(stderr, "FAILED TO CREATE WINDOW\n");
		glfwTerminate();
		return 0;
	}

	w->width = WINDOW_WIDTH;
	w->height = WINDOW_HEIGHT;
	w->resized = 0;

	glfwSetWindowUserPointer( w->handle, w );
	glfwSetFramebufferSizeCallback( w->handle, framebuffer_size_callback );

	glfwMakeContextCurrent( w->handle );

	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) ) {
		fprintf(stderr, "FAILED TO LOAD GL FUNCTIONS\n");
		return 0;
	}

	// V-SYNC
	glfwSwapInterval( 1 );

	return 1;
}

int window_should_close( struct window *w ) {
	return glfwWindowShouldClose( w->handle );
}

void window_swap_and_poll( struct window *w ) {
	w->resized = 0;
	glfwSwapBuffers( w->handle );
	glfwPollEvents();
}

void window_destroy( struct window *w ) {
	glfwDestroyWindow( w->handle );
	glfwTerminate();
}
