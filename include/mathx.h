#ifndef MATHX_H
#define MATHX_H

#define ENGINE_PI	3.14159265358979323846

static inline float clampf( float x, float lo, float hi )
{
	float r;

	r = x;
	if ( x < lo )
		r = lo;

	if ( x > hi )
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

#endif
