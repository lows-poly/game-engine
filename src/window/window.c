/*
 * window.c - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 * Callers own a `struct window` and pass it by pointer to every function here.
 */

#include <stdio.h>

#include "window.h"

static void framebuffer_size_callback( GLFWwindow *handle, int width, int height ) {
	struct window_state *w = glfwGetWindowUserPointer( handle );

	w->width = width;
	w->height = height;
	w->resized = true;

	glViewport( 0, 0, width, height );
}

int window_init( struct window_state *w, int width, int height, const char *title ) {
	if ( !glfwInit() ) {
		fprintf( stderr, "FAILED TO INIT GLFW\n" );
		return 0;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	/*
	 * MACOS - REQUIRED FOR CORE PROFILE
	 */
	#ifdef __APPLE__
		glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	#endif

	w->handle = glfwCreateWindow( width, height, title, NULL, NULL );

	if ( !w->handle ) {
		fprintf( stderr, "FAILED TO CREATE WINDOW\n" );
		glfwTerminate();
		return 0;
	}

	w->width = width;
	w->height = height;
	w->resized = false;

	glfwSetWindowUserPointer( w->handle, w );
	glfwSetFramebufferSizeCallback( w->handle, framebuffer_size_callback );

	glfwMakeContextCurrent( w->handle );

	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) ) {
		fprintf( stderr, "FAILED TO LOAD GL FUNCTIONS\n" );
		return 0;
	}

	// V-SYNC
	glfwSwapInterval( 1 );

	return 1;
}

int window_should_close( struct window_state *w ) {
	return glfwWindowShouldClose( w->handle );
}

void window_set_should_close( struct window_state *w, bool value ) {
	glfwSetWindowShouldClose( w->handle, value );
}

void window_poll_events( struct window_state *w ) {
	w->resized = false;
	glfwPollEvents();
}

void window_swap_buffers( struct window_state *w ) {
	glfwSwapBuffers( w->handle );
}

void window_destroy( struct window_state *w ) {
	glfwDestroyWindow( w->handle );
	glfwTerminate();
}
