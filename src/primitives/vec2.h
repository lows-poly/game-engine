#ifndef ENGINE_VEC2_H
#define ENGINE_VEC2_H

#include <math.h>

#define VEC_EPSILON	1e-8f
#define VEC2_ZERO_INIT	{ 0.0f, 0.0f }
#define VEC2_ZERO	((vec2)VEC2_ZERO_INIT)

typedef struct vec2 {
	union {
		struct { float x, y; };
		float raw[2];
	};
} vec2;

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

static inline float vec2_dot( vec2 a, vec2 b )
{
	return (a.x * b.x) + (a.y * b.y);
}

static inline float vec2_cross( vec2 a, vec2 b )
{
	return (a.x * b.y) - (a.y * b.x);
}

static inline float vec2_mag2( vec2 v )
{
	return vec2_dot( v, v );
}

static inline float vec2_mag( vec2 v )
{
	return sqrtf( vec2_mag2( v ) );
}

static inline vec2 vec2_add_scalar( vec2 v, float s )
{
	vec2 r;

	r.x = v.x + s;
	r.y = v.y + s;

	return r;
}

static inline vec2 vec2_sub_scalar( vec2 v, float s )
{
	vec2 r;

	r.x = v.x - s;
	r.y = v.y - s;

	return r;
}

static inline vec2 vec2_mul( vec2 a, vec2 b )
{
	vec2 r;

	r.x = a.x * b.x;
	r.y = a.y * b.y;

	return r;
}

static inline vec2 vec2_scale( vec2 v, float s )
{
	vec2 r;

	r.x = v.x * s;
	r.y = v.y * s;

	return r;
}

static inline vec2 vec2_div( vec2 a, vec2 b )
{
	vec2 r;

	r.x = a.x / b.x;
	r.y = a.y / b.y;

	return r;
}

static inline vec2 vec2_div_scalar( vec2 v, float s )
{
	vec2 r;

	r.x = v.x / s;
	r.y = v.y / s;

	return r;
}

static inline vec2 vec2_negate( vec2 v )
{
	vec2 r;

	r.x = -v.x;
	r.y = -v.y;

	return r;
}

static inline vec2 vec2_normalise( vec2 v )
{
	float mag = vec2_mag( v );

	if ( mag < VEC_EPSILON )
		return VEC2_ZERO;

	return vec2_scale( v, 1.0f / mag );
}

static inline vec2 vec2_rotate( vec2 v, float angle )
{
	vec2 r;

	float x = v.x;
	float y = v.y;

	float c = cosf( angle );
	float s = sinf( angle );

	r.x = (c * x) - (s * y);
	r.y = (s * x) + (c * y);

	return r;
}

static inline vec2 vec2_centre( vec2 a, vec2 b )
{
	return vec2_scale( vec2_add( a, b ), 0.5f );
}

static inline float vec2_dist2( vec2 a, vec2 b )
{
	float d1 = b.x - a.x;
	float d2 = b.y - a.y;

	return (d1 * d1) + (d2 * d2);
}

static inline float vec2_dist( vec2 a, vec2 b )
{
	return sqrtf( vec2_dist2( a, b ) );
}

static inline vec2 vec2_reflect( vec2 i, vec2 n )
{
	vec2 s = vec2_scale( n, 2.0f * vec2_dot( i, n ) );
	return vec2_sub( i, s );
}

#endif
