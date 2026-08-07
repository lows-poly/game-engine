#ifndef ENGINE_COLOUR_H
#define ENGINE_COLOUR_H

#include <stdio.h>

#include "mathx.h"
#include "debugf.h"

typedef struct colour {
	float r;
	float g;
	float b;
	float a;
} colour;

static const struct field_desc colour_fields[] = {
	FIELD( struct colour, r, FIELD_FLOAT ),
	FIELD( struct colour, g, FIELD_FLOAT ),
	FIELD( struct colour, b, FIELD_FLOAT ),
	FIELD( struct colour, a, FIELD_FLOAT )
};

#define COLOUR_FIELD_COUNT ( sizeof( colour_fields ) / sizeof( colour_fields[0] ) )

typedef struct icolour {
	int r;
	int g;
	int b;
} icolour;

static const struct field_desc icolour_fields[] = {
	FIELD( struct icolour, r, FIELD_INT ),
	FIELD( struct icolour, g, FIELD_INT ),
	FIELD( struct icolour, b, FIELD_INT )
};

#define ICOLOUR_FIELD_COUNT ( sizeof( icolour_fields ) / sizeof( icolour_fields[0] ) )

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

static inline void colour_to_arr( colour c, float out[4] )
{
	out[0] = c.r;
	out[1] = c.g;
	out[2] = c.b;
	out[3] = c.a;
}

static inline colour icolour_normalise( icolour c )
{
	return colour_from_rgb( c.r, c.g, c.b );
}

static inline colour colour_alpha( colour c, float alpha )
{
	c.a = clampf( alpha, 0.0f, 1.0f );
	return c;
}

static inline void colour_print( const colour c )
{
	printf( "(%.3f, %.3f, %.3f, %.3f)\n", c.r, c.g, c.b, c.a );
}

static inline void icolour_print( const icolour c )
{
	printf( "(%d, %d, %d)\n", c.r, c.g, c.b );
}

#endif
