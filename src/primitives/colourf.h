#ifndef ENGINE_COLOURF_H
#define ENGINE_COLOURF_H

#include "colour.h"

typedef struct colourf {
	float r;
	float g;
	float b;
	float a;
} colourf;

static inline colourf colour_to_float( colour c ) {
	return (colourf){
		c.r / 255.0f,
		c.g / 255.0f,
		c.b / 255.0f,
		c.a / 255.0f
	};
}

#endif
