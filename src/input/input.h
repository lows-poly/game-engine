#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <stdbool.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "input_keys_map.h"

#define INPUT_MAX_KEYS		KEY_COUNT
#define INPUT_MAX_MOUSE_BUTTONS	MOUSE_BUTTON_COUNT

struct window;

struct input_state {
	bool keys_down[INPUT_MAX_KEYS];
	bool keys_pressed[INPUT_MAX_KEYS];
	bool keys_released[INPUT_MAX_KEYS];

	bool mouse_down[INPUT_MAX_MOUSE_BUTTONS];
	bool mouse_pressed[INPUT_MAX_MOUSE_BUTTONS];
	bool mouse_released[INPUT_MAX_MOUSE_BUTTONS];

	double mouse_x;
	double mouse_y;
	double mouse_dx;
	double mouse_dy;
	double scroll_dx;
	double scroll_dy;

	bool cursor_init;
};

bool input_init( struct input_state *input, struct window *w );
void input_begin_frame( struct input_state *input );
void input_destroy( struct input_state *input, struct window *w );

static inline bool key_down( const struct input_state *input, int key )
{
	if ( !input || key < 0 || key >= INPUT_MAX_KEYS )
		return false;

	return input->keys_down[key];
}

static inline bool key_pressed( const struct input_state *input, int key )
{
	if ( !input || key < 0 || key >= INPUT_MAX_KEYS )
		return false;

	return input->keys_pressed[key];
}

static inline bool key_released( const struct input_state *input, int key )
{
	if ( !input || key < 0 || key >= INPUT_MAX_KEYS )
		return false;

	return input->keys_released[key];
}

static inline bool mouse_down( const struct input_state *input, int button )
{
	if ( !input || button < 0 || button >= INPUT_MAX_MOUSE_BUTTONS )
		return false;

	return input->mouse_down[button];
}

static inline bool mouse_pressed( const struct input_state *input, int button )
{
	if ( !input || button < 0 || button >= INPUT_MAX_MOUSE_BUTTONS )
		return false;

	return input->mouse_pressed[button];
}

static inline bool mouse_released( const struct input_state *input, int button )
{
	if ( !input || button < 0 || button >= INPUT_MAX_MOUSE_BUTTONS )
		return false;

	return input->mouse_released[button];
}

#endif
