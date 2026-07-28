#ifndef ENGINE_COLOUR_H
#define ENGINE_COLOUR_H

#include <stdint.h>

typedef struct colour {
	union {
		struct { uint8_t r, g, b, a; };
		uint8_t raw[3];
	};
} colour;

#define VINTAGE_GOLD	((colour){ 42, 48, 41 })

#define BLACK		((colour){ 0, 0, 0 })
#define WHITE		((colour){ 255, 255, 255 })
#define RED		((colour){ 255, 0, 0 })
#define GREEN		((colour){ 0, 255, 0 })
#define BLUE		((colour){ 0, 0, 255 })

static inline colour colour_make( int r, int g, int b )
{
	uint8_t _r, _g, _b;
	colour c;

	/* TODO: CLAMP */
	_r = r;
	_g = g;
	_b = b;

	c.r = _r;
	c.g = _g;
	c.b = _b;

	return c;
}

static inline void colour_to_float( colour src, float *dest )
{
	dest[0] = src.r / 255.0f;
	dest[1] = src.g / 255.0f;
	dest[2] = src.b / 255.0f;
}

#endif
