#ifndef ENGINE_VEC2_H
#define ENGINE_VEC2_H

#include <math.h>

#define VEC_EPSILON	1e-8f

typedef struct vec2 {
	float x;
	float y;
} vec2;

static inline void vec2_zero( vec2 *a )
{
	a->x = 0.0f;
	a->y = 0.0f;
}

static inline vec2 vec2_add( vec2 a, vec2 b )
{
	vec2 r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;

	return r;
}

static inline vec2 vec2_sub( vec2 a, vec2 b )
{
	vec2 r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;

	return r;
}

static inline vec2 vec2_scale( vec2 a, float scale )
{
	vec2 r;

	r.x = a.x * scale;
	r.y = a.y * scale;

	return r;
}

static inline float vec2_dot( vec2 a, vec2 b )
{
	return (a.x * b.x) + (a.y * b.y);
}

static inline float vec2_length_sq( vec2 a )
{
	return vec2_dot( a, a );
}

static inline float vec2_length( vec2 a )
{
	return sqrtf( vec2_length_sq( a ) );
}

static inline vec2 vec2_normalise( vec2 a )
{
	float len = vec2_length( a );

	if ( len < VEC_EPSILON )
		return a;

	return vec2_scale( a, 1.0f / len );
}

#endif
