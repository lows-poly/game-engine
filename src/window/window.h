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

#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <input/input.h>

/*
 * struct window - Private/internal use window state
 * @handle:            GLFW window handle
 * @frame_time_target: Target duration of a frame in seconds
 * @last_frame_time:   Timestamp of the previous frame
 * @width:             Framebuffer width
 * @height:            Framebyffer height
 * @vsync:             Vertical synchronisation
 * @resized:           Framebuffer resized
 */
struct window {
	GLFWwindow *handle;
	struct input_state *input;
	double frame_time_target;
	double last_frame_time;
	int width;
	int height;
	int vsync;
	int resized;
};

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
int window_init( struct window *w, int width, int height, const char *title );

/*
 * window_set_vsync() - Enable or disable vertical synchronisation
 * @w:       window
 * @enabled: Non-zero to enable, zero to disable
 */
void window_set_vsync( struct window *w, int enabled );

/*
 * window_set_target_fps() - Set the maximum framerate
 * @w:   window
 * @fps: Target framerate per second
 *
 * The frame-rate limit is applied when vsync is disabled.
 */
void window_set_target_fps( struct window *w, int fps );

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
int window_should_close( struct window *w );

/* 
 * window_set_should_close() - Call glfwSetWindowShouldClose()
 * @w:     window
 * @value: 0 = false, 1 = true
 */
void window_set_should_close( struct window *w, int value );

/*
 * window_end_frame() - Swap buffers and poll events
 * @w: window
 */
void window_end_frame( struct window *w );

/*
 * window_limit_fps() - Limit the frame rate to the configured value
 * @w: window
 *
 * Sleeps until the target frame duration has elapsed since the previous frame.
 */
void window_limit_fps( struct window *w );

/*
 * window_close() - Destroy glfw and close window
 * @w: window
 */
void window_close( struct window *w );

#endif
