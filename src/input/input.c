#include <string.h>

#include "input.h"

static void key_callback( GLFWwindow *window, int key, int scancode, int action,
                          int mods ) {
	struct input_state *input;

	(void) scancode;
	(void) mods;

	input = glfwGetWindowUserPointer( window );

	if ( !input )
		return;

	if ( key < 0 || key >= INPUT_MAX_KEYS )
		return;

	if ( action == GLFW_PRESS )
		input->keys_curr[key] = true;
	else if ( action == GLFW_RELEASE )
		input->keys_curr[key] = false;
}

static void mouse_button_callback( GLFWwindow *window, int button, int action,
                                   int mods ) {
	struct input_state *input;

	(void) mods;

	input = glfwGetWindowUserPointer( window );
	if ( !input )
		return;

	if ( button < 0 || button >= INPUT_MAX_MOUSE_BUTTONS )
		return;

	if ( action == GLFW_PRESS )
		input->mouse_curr[button] = true;
	else if ( action == GLFW_RELEASE )
		input->mouse_curr[button] = false;
}

static void cursor_pos_callback( GLFWwindow *window, double xpos, double ypos ) {
	struct input_state *input;

	input = glfwGetWindowUserPointer( window );

	if ( !input )
		return;

	input->mouse_dx += xpos - input->mouse_x;
	input->mouse_dy += ypos - input->mouse_y;

	input->mouse_x = xpos;
	input->mouse_y = ypos;
}


static void scroll_callback( GLFWwindow *window, double xoffset, double yoffset ) {
	struct input_state *input;

	input = glfwGetWindowUserPointer( window );

	if ( !input )
		return;

	input->scroll_dx += xoffset;
	input->scroll_dy += yoffset;
}

void input_init( struct input_state *input, GLFWwindow *window ) {
	memset( input, 0, sizeof( *input ) );

	glfwSetWindowUserPointer( window, input );

	glfwSetKeyCallback( window, key_callback );
	glfwSetMouseButtonCallback( window, mouse_button_callback );
	glfwSetCursorPosCallback( window, cursor_pos_callback );
	glfwSetScrollCallback( window, scroll_callback );
	/*
	 * prime mouse_x/mouse_y so the first cursor_pos_callback
	 * doesn't produce a huge jump.
	 */
	glfwGetCursorPos( window, &input->mouse_x, &input->mouse_y );
}

void input_update( struct input_state *input ) {
	memcpy( input->keys_prev, input->keys_curr, sizeof( input->keys_curr ) );
	memcpy( input->mouse_prev, input->mouse_curr, sizeof( input->mouse_curr ) );

	input->mouse_dx = 0.0;
	input->mouse_dy = 0.0;

	input->scroll_dx = 0.0;
	input->scroll_dy = 0.0;
}

void input_destroy( struct input_state *input, GLFWwindow *window ) {
	(void) input;

	glfwSetKeyCallback( window, NULL );
	glfwSetMouseButtonCallback( window, NULL );
	glfwSetCursorPosCallback( window, NULL );
	glfwSetScrollCallback( window, NULL );

	glfwSetWindowUserPointer( window, NULL );
}
