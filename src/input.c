#include <string.h>
#include "input.h"

static void key_callback( GLFWwindow *window, int key, int scancode, int action,
                          int mods ) {
	struct input_state *input;

	(void) scancode;
	(void) mods;

	input = glfwWindowUserPointer( window );

	if ( !input )
		return;

	if ( key < 0 || key >= INPUT_MAX_KEYS )
		return;

	if ( action == GLFW_PRESS )
		input->keys_curr[key] = true;
	else if ( action == GLFW_RELEASE )
		input->keys_curr[key] = false;
}

static void cursor_pos_callback( GLFWwindow *window, double xpos, double ypos ) {
	struct input_state *input;

	input = glfwWindowUserPointer( window );

	if ( !input )
		return;

	input->mouse_dx += xpos - input->mouse_x;
	input->mouse_dy += ypos - input->mouse_y;

	input->mouse_x = xpos;
	input->mouse_y = ypos;
}


static void scroll_callback
