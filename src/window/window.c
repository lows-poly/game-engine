#include <string.h>

#include <glad/glad.h>

#include "window.h"

#define WINDOW_GL_MAJOR	4
#define WINDOW_GL_MINOR	1

static void framebuffer_size_callback( GLFWwindow *handle, int width, int height )
{
	struct window *w;
	
	w = glfwGetWindowUserPointer( handle );
	if ( !w )
		return;

	w->framebuffer_width = width;
	w->framebuffer_height = height;
	w->resized = true;

	glViewport( 0, 0, width, height );
}

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
bool window_init( struct window *w, int width, int height, const char *title )
{
	if ( !w || !title || width <= 0 || height <= 0 )
		return false;

	memset( w, 0, sizeof( *w ) );
	w->input = NULL;

	if ( !glfwInit() )
		return false;

	w->glfw_init = true;

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, WINDOW_GL_MAJOR );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, WINDOW_GL_MINOR );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifdef __APPLE__
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#endif

	w->handle = glfwCreateWindow( width, height, title, NULL, NULL );
	if ( !w->handle )
		goto ERR_GLFW;

	glfwMakeContextCurrent( w->handle );

	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
		goto ERR_WINDOW;

	glfwSetWindowUserPointer( w->handle, w );
	glfwSetFramebufferSizeCallback( w->handle, framebuffer_size_callback );

	glfwGetFramebufferSize( w->handle, &w->framebuffer_width, &w->framebuffer_height );
	glViewport( 0, 0, w->framebuffer_width, w->framebuffer_height );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	window_set_vsync( w, false );

	return true;

ERR_WINDOW:
	glfwDestroyWindow( w->handle );
	w->handle = NULL;

ERR_GLFW:
	glfwTerminate();
	w->glfw_init = false;

	return false;
}

void window_destroy( struct window *w )
{
	if ( !w )
		return;

	if ( w->handle ) {
		glfwSetWindowUserPointer( w->handle, NULL );
		glfwDestroyWindow( w->handle );
		w->handle = NULL;
	}

	if ( w->glfw_init ) {
		glfwTerminate();
		w->glfw_init = false;
	}

	w->framebuffer_width = 0;
	w->framebuffer_height = 0;
	w->resized = false;
	w->vsync = false;
}

void window_poll_events( void )
{
	glfwPollEvents();
}

void window_swap_buffers( const struct window *w )
{
	if ( !w || !w->handle )
		return;

	glfwSwapBuffers( w->handle );
}

bool window_should_close( const struct window *w )
{
	if ( !w || !w->handle )
		return true;

	return glfwWindowShouldClose( w->handle ) != 0;
}

void window_set_should_close( struct window *w, bool should_close )
{
	if ( !w || !w->handle )
		return;

	glfwSetWindowShouldClose( w->handle, should_close ? GLFW_TRUE : GLFW_FALSE );
}

void window_set_vsync( struct window *w, bool enabled )
{
	if ( !w || !w->handle )
		return;

	glfwMakeContextCurrent( w->handle );
	glfwSwapInterval( enabled ? 1 : 0 );
	w->vsync = enabled;
}

bool window_take_resize( struct window *w, int *width, int *height )
{
	if ( !w || !width || !height || !w->resized )
		return false;

	*width = w->framebuffer_width;
	*height = w->framebuffer_height;
	w->resized = false;

	return true;
}
