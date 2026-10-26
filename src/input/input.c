#include <string.h>

#include <GLFW/glfw3.h>

#include "input.h"
#include "window/window.h"

static void key_callback( GLFWwindow *handle, int key, int scancode, int action,
			  int mods )
{
	struct window *w;
	struct input_state *input;

	(void)scancode;
	(void)mods;

	w = glfwGetWindowUserPointer( handle );
	if ( !w || !w->input || key < 0 || key >= INPUT_MAX_KEYS )
		return;

	input = w->input;

	if ( action == GLFW_PRESS ) {
		if ( !input->keys_down[key] )
			input->keys_pressed[key] = true;

		input->keys_down[key] = true;
	} else if ( action == GLFW_RELEASE ) {
		if ( input->keys_down[key] )
			input->keys_released[key] = true;

		input->keys_down[key] = false;
	}
}

static void mouse_button_callback( GLFWwindow *handle, int button, int action,
				   int mods )
{
	struct window *w;
	struct input_state *input;

	(void)mods;

	w = glfwGetWindowUserPointer( handle );
	if ( !w || !w->input || button < 0 ||
	     button >= INPUT_MAX_MOUSE_BUTTONS )
		return;

	input = w->input;

	if ( action == GLFW_PRESS ) {
		if ( !input->mouse_down[button] )
			input->mouse_pressed[button] = true;

		input->mouse_down[button] = true;
	} else if ( action == GLFW_RELEASE ) {
		if ( input->mouse_down[button] )
			input->mouse_released[button] = true;

		input->mouse_down[button] = false;
	}
}

static void cursor_pos_callback( GLFWwindow *handle, double xpos, double ypos )
{
	struct window *w;
	struct input_state *input;

	w = glfwGetWindowUserPointer( handle );
	if ( !w || !w->input )
		return;

	input = w->input;

	if ( input->cursor_init ) {
		input->mouse_dx += xpos - input->mouse_x;
		input->mouse_dy += ypos - input->mouse_y;
	}

	input->mouse_x = xpos;
	input->mouse_y = ypos;
	input->cursor_init = true;
}

static void scroll_callback( GLFWwindow *handle, double xoffset, double yoffset )
{
	struct window *w;

	w = glfwGetWindowUserPointer( handle );
	if ( !w || !w->input )
		return;

	w->input->scroll_dx += xoffset;
	w->input->scroll_dy += yoffset;
}

static void focus_callback( GLFWwindow *handle, int focused )
{
	struct window *w;
	struct input_state *input;
	int i;

	if ( focused )
		return;

	w = glfwGetWindowUserPointer( handle );
	if ( !w || !w->input )
		return;

	input = w->input;

	for ( i = 0; i < INPUT_MAX_KEYS; i++ ) {
		if ( input->keys_down[i] )
			input->keys_released[i] = true;

		input->keys_down[i] = false;
	}

	for ( i = 0; i < INPUT_MAX_MOUSE_BUTTONS; i++ ) {
		if ( input->mouse_down[i] )
			input->mouse_released[i] = true;

		input->mouse_down[i] = false;
	}

	input->cursor_init = false;
}

bool input_init( struct input_state *input, struct window *w )
{
	if ( !input || !w || !w->handle || (w->input && w->input != input) )
		return false;

	memset( input, 0, sizeof( *input ) );
	w->input = input;

	glfwSetKeyCallback( w->handle, key_callback );
	glfwSetMouseButtonCallback( w->handle, mouse_button_callback );
	glfwSetCursorPosCallback( w->handle, cursor_pos_callback );
	glfwSetScrollCallback( w->handle, scroll_callback );
	glfwSetWindowFocusCallback( w->handle, focus_callback );

	glfwGetCursorPos( w->handle, &input->mouse_x, &input->mouse_y );
	input->cursor_init = true;

	return true;
}

void input_begin_frame( struct input_state *input )
{
	if ( !input )
		return;

	memset( input->keys_pressed, 0, sizeof( input->keys_pressed ) );
	memset( input->keys_released, 0, sizeof( input->keys_released ) );
	memset( input->mouse_pressed, 0, sizeof( input->mouse_pressed ) );
	memset( input->mouse_released, 0, sizeof( input->mouse_released ) );

	input->mouse_dx = 0.0;
	input->mouse_dy = 0.0;
	input->scroll_dx = 0.0;
	input->scroll_dy = 0.0;
}

void input_destroy( struct input_state *input, struct window *w )
{
	if ( !input )
		return;

	if ( w && w->input == input ) {
		if ( w->handle ) {
			glfwSetKeyCallback( w->handle, NULL );
			glfwSetMouseButtonCallback( w->handle, NULL );
			glfwSetCursorPosCallback( w->handle, NULL );
			glfwSetScrollCallback( w->handle, NULL );
			glfwSetWindowFocusCallback( w->handle, NULL );
		}

		w->input = NULL;
	}

	memset( input, 0, sizeof( *input ) );
}
