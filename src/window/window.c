/*
 * window.c - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 *
 * NOTE:
 *	Using int as boolean: 0 = false, 1 = true
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

static struct wstate {
	GLFWwindow *handle;
	double frame_time_target;
	double last_frame_time;
	int width;
	int height;
	int vsync;
	int resized;
} wstate;

static void _warn( const char *msg )
{
	fprintf( stderr, "WINDOW: %s\n", msg );
}

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

static void window_limit_fps( void )
{
	double now;
	double elapsed;
	double remaining;

	if ( wstate.frame_time_target <= 0.0 )
		return;

	now = glfwGetTime();
	elapsed = now - wstate.last_frame_time;
	remaining = wstate.frame_time_target - elapsed;

	if ( remaining > 0.001 )
		sleep_ms( (remaining - 0.001) * 1000.0 );

	do {
		now = glfwGetTime();
		elapsed = now - wstate.last_frame_time;
	} while ( elapsed < wstate.frame_time_target );

	wstate.last_frame_time = now;
}

static void framebuffer_size_callback( GLFWwindow *handle, int width, int height )
{
	if ( !wstate.handle )
		return;

	wstate.width = width;
	wstate.height = height;
	wstate.resized = 1;

	glViewport( 0, 0, width, height );
}

int window_init( int width, int height, const char *title )
{
	if ( !glfwInit() ) {
		_warn("FAILED TO INIT GLFW");
		return 0;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, WINDOW_GL_MAJOR );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, WINDOW_GL_MINOR );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	wstate.handle = glfwCreateWindow( width, height, title, NULL, NULL );

	if ( !wstate.handle ) {
		_warn("FAILED TO CREATE WINDOW");
		glfwTerminate();
		return 0;
	}

	wstate.width = width;
	wstate.height = height;
	wstate.resized = 0;
	wstate.frame_time_target = 0.0;
	wstate.vsync = 0;

	glfwSetFramebufferSizeCallback( wstate.handle, framebuffer_size_callback );
	glfwMakeContextCurrent( wstate.handle );

	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) ) {
		_warn("FAILED TO LOAD GL FUNCTIONS");
		glfwDestroyWindow( wstate.handle );
		wstate.handle = NULL;
		glfwTerminate();
		return 0;
	}

	wstate.last_frame_time = glfwGetTime();

	return 1;
}

void window_set_vsync( int enabled )
{
	int _val;
	_val = enabled ? 1 : 0;

	if ( !wstate.handle )
		return;

	wstate.vsync = _val;
	glfwSwapInterval( _val );
}

void window_set_target_fps( int fps )
{
	assert( fps != NULL );

	if ( wstate.vsync )
		_warn("FPS LIMIT AND VSYNC IS ENABLED");

	wstate.frame_time_target = 1.0 / (double)fps;
}

int window_should_close( void )
{
	if ( !wstate.handle )
		return 1;

	glfwSwapBuffers( wstate.handle );
	window_limit_fps();
	glfwPollEvents();
	wstate.resized = 0;

	return glfwWindowShouldClose( wstate.handle );
}

void window_close( void )
{
	if ( wstate.handle ) {
		glfwDestroyWindow( wstate.handle );
		wstate.handle = NULL;
	}

	glfwTerminate();
}
