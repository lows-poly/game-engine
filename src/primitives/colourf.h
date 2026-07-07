#ifndef ENGINE_COLOURF_H
#define ENGINE_COLOURF_H

#include "colour.h"

typedef struct colourf {
	float r;
	float g;
	float b;
	float a;
} colourf;

static inline colourf colour_to_float( colour c )
{
	colourf cf;

	cf.r = c.r / 255.0f;
	cf.g = c.g / 255.0f;
	cf.b = c.b / 255.0f;
	cf.a = c.a / 255.0f;

	return cf;
}

#endif
