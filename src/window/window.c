/*
 * window.h - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 *
 * NOTE:
 * Using int as boolean:
 *  0 = false
 *  1 = true
 */

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include "window.h"

#define WINDOW_GL_MAJOR 4
#define WINDOW_GL_MINOR 1

/*
 * _warn() - Print a window subsystem warning
 * @msg: Warning mesage
 */
static void _warn( const char *msg )
{
	fprintf( stderr, "WINDOW: %s\n", msg );
}

/*
 * _sleep_ms() - Sleep for the specified number of milliseconds
 * @ms:	Duration in milliseconds
 */
static void _sleep_ms( double ms )
{
#ifdef _WIN32
	Sleep( (DWORD)ms );
#else
	struct timespec ts;
	ts.tv_sec = (time_t)(ms / 1000.0);
	ts.tv_nsec = (long)((ms - (ts.tv_sec * 1000.0)) * 1e6);
	nanosleep( &ts, NULL );
#endif
}

/*
 * window_limit_fps() - Limit the frame rate to the configured value
 * @w: window
 *
 * Sleeps until the target frame duration has elapsed since the previous frame.
 */
static void window_limit_fps( struct window *w )
{
	double now;
	double elapsed;
	double remaining;

	if ( w->frame_time_target <= 0.0 )
		return;

	now = glfwGetTime();
	elapsed = now - w->last_frame_time;
	remaining = w->frame_time_target - elapsed;

	if ( remaining > 0.001 )
		_sleep_ms( (remaining - 0.001) * 1000.0 );

	do {
		now = glfwGetTime();
		elapsed = now - w->last_frame_time;
	} while ( elapsed < w->frame_time_target );

	w->last_frame_time = now;
}

/* 
 * framebuffer_size_callback() - Handle framebuffer resizing
 * @handle: GLFW window
 * @width:  New framebuffer width
 * @height: New framebuffer height
 */
static void framebuffer_size_callback( GLFWwindow *handle, int width, int height )
{
	struct window *w;

	w = glfwGetWindowUserPointer( handle );
	if ( !w )
		return;

	w->width = width;
	w->height = height;
	w->resized = 1;

	glViewport( 0, 0, width, height );
}

/*
 * window_init() - Initialise GLFW and create window
 * @w:      window
 * @width:  Window width
 * @height: Window height
 * @title:  Window title
 *
 * Initialise glfw, create the window, setup framebuffer callback, create the
 * OpenGL context, and load OpenGL functions through glad
 *
 * Return:
 * 0 = Failure
 * 1 = Success
 */
int window_init( struct window *w, int width, int height, const char *title )
{
	if ( !glfwInit() ) {
		_warn("FAILED TO INIT GLFW");
		return 0;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, WINDOW_GL_MAJOR );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, WINDOW_GL_MINOR );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	w->handle = glfwCreateWindow( width, height, title, NULL, NULL );

	if ( !w->handle ) {
		_warn("FAILED TO CREATE WINDOW");
		glfwTerminate();
		return 0;
	}

	w->width = width;
	w->height = height;
	w->resized = 0;
	w->frame_time_target = 0.0;
	w->vsync = 0;

	glfwSetWindowUserPointer( w->handle, w );
	glfwSetFramebufferSizeCallback( w->handle, framebuffer_size_callback );
	glfwMakeContextCurrent( w->handle );

	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) ) {
		_warn("FAILED TO LOAD GL FUNCTIONS");
		glfwDestroyWindow( w->handle );
		w->handle = NULL;
		glfwTerminate();
		return 0;
	}

	w->last_frame_time = glfwGetTime();

	return 1;
}

/*
 * window_set_vsync() - Enable or disable vertical synchronisation
 * @w:       window
 * @enabled: Non-zero to enable, zero to disable
 */
void window_set_vsync( struct window *w, int enabled )
{
	int _val;
	_val = enabled ? 1 : 0;

	if ( !w->handle )
		return;

	w->vsync = _val;
	glfwSwapInterval( _val );
}

/*
 * window_set_target_fps() - Set the maximum framerate
 * @w:   window
 * @fps: Target framerate per second
 *
 * The frame-rate limit is applied when vsync is disabled.
 */
void window_set_target_fps( struct window *w, int fps )
{
	if ( fps <= 0 ) {
		_warn("INVALID TARGET FPS");
		return;
	}

	if ( w->vsync ) {
		_warn("UNABLE TO SET TARGET FPS - VSYNC IS ENABLED");
		return;
	}

	w->frame_time_target = 1.0 / (double)fps;
}

/*
 * window_should_close() - Process the current frame and check for closure
 * @w: window
 *
 * Return whether the window should close.
 *
 * Return:
 * 0 - Window should remain open
 * 1 - Window should close
 */
int window_should_close( struct window *w )
{
	if ( !w->handle )
		return 1;

	return glfwWindowShouldClose( w->handle );
}

/*
 * window_close() - Destroy glfw and close window
 * @w: window
 */
void window_close( struct window *w )
{
	if ( w->handle ) {
		glfwDestroyWindow( w->handle );
		w->handle = NULL;
	}

	glfwTerminate();
}
