#ifndef MATHX_H
#define MATHX_H

#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define PI	3.14159265358979323846
#define TWO_PI	(PI * 2.0)

static inline int min( int a, int b )
{
	return (a < b) ? a : b;
}

static inline float minf( float a, float b )
{
	return (a < b) ? a : b;
}

static inline int max( int a, int b )
{
	return (a > b) ? a : b;
}

static inline float maxf( float a, float b )
{
	return (a > b) ? a : b;
}

static inline float clampf( float x, float lo, float hi )
{
	float r;

	assert( lo <= hi );

	r = x;
	if ( isnan( x ) )
		r = lo;
	else if ( x < lo )
		r = lo;
	else if ( x > hi )
		r = hi;

	return r;
}

static inline int clamp( int x, int lo, int hi )
{
	int r;

	r = x;
	if ( x < lo )
		r = lo;

	if ( x > hi )
		r = hi;

	return r;
}

static inline float randf( float min, float max )
{
	float scale;
	
	scale = (float)rand() / (float)RAND_MAX;
	return min + scale * (max - min);
}

static inline float wrapf( float v, float max )
{
	if ( v < 0.0f )
		return v + max;

	if ( v >= max )
		return v - max;

	return v;
}

#endif
