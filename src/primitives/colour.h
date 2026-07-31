#ifndef ENGINE_COLOUR_H
#define ENGINE_COLOUR_H

#include <stdint.h>

#include "mathx.h"

typedef struct colour {
	union {
		struct { uint8_t r, g, b; };
		uint8_t raw[3];
	};
} colour;

typedef struct colourf {
	union {
		struct { float r, g, b, a; };
		float raw[4];
	};
} colourf;

#define VINTAGE_GOLD	((colour){ 42, 48, 41 })

#define BLACK		((colour){ 0, 0, 0 })
#define WHITE		((colour){ 255, 255, 255 })
#define RED		((colour){ 255, 0, 0 })
#define GREEN		((colour){ 0, 255, 0 })
#define BLUE		((colour){ 0, 0, 255 })
#define CYAN            ((colour){ 0, 150, 150 })

static inline colour colour_make( int r, int g, int b )
{
	colour c;

	c.r = (uint8_t)clamp( r, 0, 255 );
	c.g = (uint8_t)clamp( g, 0, 255 );
	c.b = (uint8_t)clamp( b, 0, 255 );

	return c;
}

static inline colourf colourf_make( colour c )
{
	colourf cf;

	cf.r = c.r / 255.0f;
	cf.g = c.g / 255.0f;
	cf.b = c.b / 255.0f;
	cf.a = 1.0f;

	return cf;
}

static inline void colour_to_float( colour src, float *dest )
{
	dest[0] = src.r / 255.0f;
	dest[1] = src.g / 255.0f;
	dest[2] = src.b / 255.0f;
}

#endif
