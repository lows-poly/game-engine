#ifndef ENGINE_VEC4_H
#define ENGINE_VEC4_H

#include <math.h>
#include "vec3.h"

#define VEC4_ZERO_INIT	{ 0.0f, 0.0f, 0.0f, 0.0f }
#define VEC4_ZERO	((vec4)VEC4_ZERO_INIT)

typedef struct vec4 {
	union {
		struct { float x, y, z, w; };
		float raw[4];
	};
} vec4;

static inline vec4 vec4_add( vec4 a, vec4 b )
{
	vec4 r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	r.w = a.w + b.w;

	return r;
}

static inline vec4 vec4_sub( vec4 a, vec4 b )
{
	vec4 r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	r.w = a.w - b.w;

	return r;
}

static inline float vec4_dot( vec4 a, vec4 b )
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

static inline float vec4_mag2( vec4 v )
{
	return vec4_dot( v, v );
}

static inline float vec4_mag( vec4 v )
{
	return sqrtf( vec4_mag2( v ) );
}

static inline vec4 vec4_add_scalar( vec4 v, float s )
{
	vec4 r;

	r.x = v.x + s;
	r.y = v.y + s;
	r.z = v.z + s;
	r.w = v.w + s;

	return r;
}

static inline vec4 vec4_sub_scalar( vec4 v, float s )
{
	vec4 r;

	r.x = v.x - s;
	r.y = v.y - s;
	r.z = v.z - s;
	r.w = v.w - s;

	return r;
}

static inline vec4 vec4_mul( vec4 a, vec4 b )
{
	vec4 r;

	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;
	r.w = a.w * b.w;

	return r;
}

static inline vec4 vec4_scale( vec4 v, float s )
{
	vec4 r;

	r.x = v.x * s;
	r.y = v.y * s;
	r.z = v.z * s;
	r.w = v.w * s;

	return r;
}

static inline vec4 vec4_div( vec4 a, vec4 b )
{
	vec4 r;

	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;
	r.w = a.w / b.w;

	return r;
}

static inline vec4 vec4_div_scalar( vec4 v, float s )
{
	vec4 r;

	r.x = v.x / s;
	r.y = v.y / s;
	r.z = v.z / s;
	r.w = v.w / s;

	return r;
}

static inline vec4 vec4_negate( vec4 v )
{
	vec4 r;

	r.x = -v.x;
	r.y = -v.y;
	r.z = -v.z;
	r.w = -v.w;

	return r;
}

static inline vec4 vec4_normalise( vec4 v )
{
	float mag = vec4_mag( v );

	if ( mag < VEC_EPSILON )
		return VEC4_ZERO;

	return vec4_scale( v, 1.0f / mag );
}

static inline float vec4_angle( vec4 a, vec4 b )
{
	float mag_a, mag_b, dot;

	mag_a = vec4_mag( a );
	mag_b = vec4_mag( b );

	if ( mag_a < VEC_EPSILON || mag_b < VEC_EPSILON )
		return 0.0f;

	dot = vec4_dot( a, b ) / (mag_a * mag_b);

	if ( dot > 1.0f )
		return 0.0f;
	else if ( dot < -1.0f )
		return VEC_PI;

	return acosf( dot );
}

static inline vec4 vec4_proj( vec4 a, vec4 b )
{
	return vec4_scale( b, vec4_dot( a, b ) / vec4_mag2( b ) );
}

static inline vec4 vec4_centre( vec4 a, vec4 b )
{
	return vec4_scale( vec4_add( a, b ), 0.5f );
}

static inline float vec4_dist2( vec4 a, vec4 b )
{
	float d1 = b.x - a.x;
	float d2 = b.y - a.y;
	float d3 = b.z - a.z;
	float d4 = b.w - a.w;

	return (d1 * d1) + (d2 * d2) + (d3 * d3) + (d4 * d4);
}

static inline float vec4_dist( vec4 a, vec4 b )
{
	return sqrtf( vec4_dist2( a, b ) );
}

static inline vec4 vec4_reflect( vec4 i, vec4 n )
{
	vec4 s = vec4_scale( n, 2.0f * vec4_dot( i, n ) );
	return vec4_sub( i, s );
}
#endif
