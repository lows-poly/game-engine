#include <string.h>

#include "window/window.h"
#include "input.h"

static void key_callback( GLFWwindow *window, int key, int scancode, int action,
                          int mods )
{
	struct window *w;

	(void) scancode;
	(void) mods;

	w = glfwGetWindowUserPointer( window );

	if ( !w || !w->input )
		return;

	if ( key < 0 || key >= INPUT_MAX_KEYS )
		return;

	if ( action == GLFW_PRESS )
		w->input->keys_curr[key] = true;
	else if ( action == GLFW_RELEASE )
		w->input->keys_curr[key] = false;
}

static void mouse_button_callback( GLFWwindow *window, int button, int action,
                                   int mods )
{
	struct window *w;

	(void) mods;

	w = glfwGetWindowUserPointer( window );

	if ( !w || !w->input )
		return;

	if ( button < 0 || button >= INPUT_MAX_MOUSE_BUTTONS )
		return;

	if ( action == GLFW_PRESS )
		w->input->mouse_curr[button] = true;
	else if ( action == GLFW_RELEASE )
		w->input->mouse_curr[button] = false;
}

static void cursor_pos_callback( GLFWwindow *window, double xpos, double ypos )
{
	struct window *w;

	w = glfwGetWindowUserPointer( window );

	if ( !w || !w->input )
		return;

	w->input->mouse_dx += xpos - w->input->mouse_x;
	w->input->mouse_dy += ypos - w->input->mouse_y;

	w->input->mouse_x = xpos;
	w->input->mouse_y = ypos;
}


static void scroll_callback( GLFWwindow *window, double xoffset, double yoffset )
{
	struct window *w;

	w = glfwGetWindowUserPointer( window );

	if ( !w || !w->input )
		return;

	w->input->scroll_dx += xoffset;
	w->input->scroll_dy += yoffset;
}

void input_init( struct input_state *input, struct window *w )
{
	memset( input, 0, sizeof( *input ) );
	w->input = input;

	glfwSetKeyCallback( w->handle, key_callback );
	glfwSetMouseButtonCallback( w->handle, mouse_button_callback );
	glfwSetCursorPosCallback( w->handle, cursor_pos_callback );
	glfwSetScrollCallback( w->handle, scroll_callback );

	glfwGetCursorPos( w->handle, &input->mouse_x, &input->mouse_y );
}

void input_update( struct input_state *input )
{
	memcpy( input->keys_prev, input->keys_curr, sizeof( input->keys_curr ) );
	memcpy( input->mouse_prev, input->mouse_curr, sizeof( input->mouse_curr ) );

	input->mouse_dx = 0.0;
	input->mouse_dy = 0.0;

	input->scroll_dx = 0.0;
	input->scroll_dy = 0.0;
}

void input_destroy( struct input_state *input, struct window *w )
{
	(void) input;

	glfwSetKeyCallback( w->handle, NULL );
	glfwSetMouseButtonCallback( w->handle, NULL );
	glfwSetCursorPosCallback( w->handle, NULL );
	glfwSetScrollCallback( w->handle, NULL );

	w->input = NULL;
}
