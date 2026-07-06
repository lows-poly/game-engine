#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <stdbool.h>

#define INPUT_MAX_KEYS		  GLFW_KEY_LAST + 1
#define INPUT_MAX_MOUSE_BUTTONS	  GLFW_MOUSE_BUTTON_LAST + 1

struct input_state {
	bool keys_curr[INPUT_MAX_KEYS];
	bool keys_prev[INPUT_MAX_KEYS];

	bool mouse_curr[INPUT_MAX_MOUSE_BUTTONS];
	bool mouse_prev[INPUT_MAX_MOUSE_BUTTONS];

	double mouse_x;
	double mouse_y;
	double mouse_dx;
	double mouse_dy;

	double scroll_dx;
	double scroll_dy;
};

void input_init( struct input_state *input, GLFWwindow *window );
void input_update( struct input_state *input );
static inline bool input_key_down( const struct input_state *input, int key ) {
	return input->keys_curr[key];
}

static inline bool input_key_pressed( const struct input_state *input, int key ) {
	return input->keys_curr[key] && !input->keys_prev[key];
}

static inline bool input_key_rel( const struct input_state *input, int key ) {
	return !input->keys_curr[key] && input->keys_prev[key];
}

static inline bool input_mouse_down( const struct input_state *input, int button ) {
	return input->mouse_curr[button];
}

static inline bool input_mouse_pressed( const struct input_state *input, int button ) {
	return input->mouse_curr[button] && !input->mouse_prev[button];
}

static inline bool input_mouse_rel( const struct input_state *input, int button ) {
	return !input->mouse_curr[button] && input->mouse_prev[button];
}

#endif
