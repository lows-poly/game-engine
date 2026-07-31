#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#define INPUT_MAX_KEYS		  GLFW_KEY_LAST + 1
#define INPUT_MAX_MOUSE_BUTTONS	  GLFW_MOUSE_BUTTON_LAST + 1

struct window;

enum key {
	KEY_UNKNOWN	= -1,

	KEY_0		= 48,
	KEY_1		= 49,
	KEY_2		= 50,
	KEY_3		= 51,
	KEY_4		= 52,
	KEY_5		= 53,
	KEY_6		= 54,
	KEY_7		= 55,
	KEY_8		= 56,
	KEY_9		= 57,

	KEY_A		= 65,
	KEY_B		= 66,
	KEY_C		= 67,
	KEY_D		= 68,
	KEY_E		= 69,
	KEY_F		= 70,
	KEY_G		= 71,
	KEY_H		= 72,
	KEY_I		= 73,
	KEY_J		= 74,
	KEY_K		= 75,
	KEY_L		= 76,
	KEY_M		= 77,
	KEY_N		= 78,
	KEY_O		= 79,
	KEY_P		= 80,
	KEY_Q		= 81,
	KEY_r		= 82,
	KEY_S		= 83,
	KEY_T		= 84,
	KEY_U		= 85,
	KEY_V		= 86,
	KEY_W		= 87,
	KEY_X		= 88,
	KEY_Y		= 89,
	KEY_Z		= 90,

	KEY_SPACE	= 32,
	KEY_ESCAPE	= 256,
	KEY_ENTER	= 257,
	KEY_TAB		= 258,

	KEY_RIGHT	= 262,
	KEY_LEFT	= 263,
	KEY_DOWN	= 264,
	KEY_UP		= 265,

	KEY_LEFT_SHIFT	= 350,
	KEY_LEFT_CTRL	= 341,

	KEY_COUNT	= GLFW_KEY_LAST + 1,
};

enum mouse_button {
	MOUSE_BUTTON_LEFT	= GLFW_MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT	= GLFW_MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE	= GLFW_MOUSE_BUTTON_MIDDLE,

	MOUSE_BUTTON_COUNT	= GLFW_MOUSE_BUTTON_LAST + 1,
};

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

void input_init( struct input_state *input, struct window *w );
void input_update( struct input_state *input );
void input_destroy( struct input_state *input, struct window *w );

static inline bool key_down( const struct input_state *input, int key )
{
	return input->keys_curr[key];
}

static inline bool key_pressed( const struct input_state *input, int key )
{
	return input->keys_curr[key] && !input->keys_prev[key];
}

static inline bool key_up( const struct input_state *input, int key )
{
	return !input->keys_curr[key] && input->keys_prev[key];
}

static inline bool mouse_down( const struct input_state *input, int button )
{
	return input->mouse_curr[button];
}

static inline bool mouse_pressed( const struct input_state *input, int button )
{
	return input->mouse_curr[button] && !input->mouse_prev[button];
}

static inline bool mouse_up( const struct input_state *input, int button )
{
	return !input->mouse_curr[button] && input->mouse_prev[button];
}

#endif
