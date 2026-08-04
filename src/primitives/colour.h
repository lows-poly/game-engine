#ifndef ENGINE_COLOUR_H
#define ENGINE_COLOUR_H

#include <stdio.h>

#include "mathx.h"

typedef struct colour {
	union {
		struct { float r, g, b, a; };
		float raw[4];
	};
} colour;

typedef struct icolour {
	int r;
	int g;
	int b;
} icolour;

#define VINTAGE_GOLD    ((colour){ 0.164f, 0.188f, 0.16f, 1.0f })

#define BLACK           ((colour){ 0.0f, 0.0f, 0.0f, 1.0f })
#define WHITE           ((colour){ 1.0f, 1.0f, 1.0f, 1.0f })
#define RED             ((colour){ 1.0f, 0.0f, 0.0f, 1.0f })
#define GREEN           ((colour){ 0.0f, 1.0f, 0.0f, 1.0f })
#define BLUE            ((colour){ 0.0f, 0.0f, 1.0f, 1.0f })
#define CYAN            ((colour){ 0.0f, 0.588f, 0.588f, 1.0f })

static inline colour colour_make( float r, float g, float b, float a )
{
	colour c;

	c.r = clampf( r, 0.0f, 1.0f );
	c.g = clampf( g, 0.0f, 1.0f );
	c.b = clampf( b, 0.0f, 1.0f );
	c.a = clampf( a, 0.0f, 1.0f );

	return c;
}

static inline colour colour_from_rgb( int r, int g, int b )
{
	colour c;

	c.r = clamp( r, 0, 255 ) / 255.0f;
	c.g = clamp( g, 0, 255 ) / 255.0f;
	c.b = clamp( b, 0, 255 ) / 255.0f;
	c.a = 1.0f;

	return c;
}

static inline icolour colour_to_rgb( colour c )
{
	icolour ic;

	ic.r = (int)( c.r * 255.0f );
	ic.g = (int)( c.g * 255.0f );
	ic.b = (int)( c.b * 255.0f );

	return ic;
}

static inline icolour icolour_normalise( icolour c )
{
	return colour_from_rgb( c.a, c.g, c.b );
}

static inline colour colour_alpha( colour c, float alpha )
{
	c.a = clampf( alpha, 0.0f, 1.0f );
	return c;
}

static void colour_print( const colour c )
{
	printf( "(%.3f, %.3f, %.3f, %.3f)\n", c.r, c.g, c.b, c.a );
}

static void icolour_print( const icolour c )
{
	printf( "(%d, %d, %d)\n", c.r, c.g, c.b );
}

#endif
