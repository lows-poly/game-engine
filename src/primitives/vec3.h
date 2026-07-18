#ifndef ENGINE_VEC3_H
#define ENGINE_VEC3_H

#include <math.h>

#define VEC_PI		3.14159265358979323846f
#define VEC_EPSILON	1e-8f
#define VEC3_ZERO_INIT	{ 0.0f, 0.0f, 0.0f }
#define VEC3_ZERO	((vec3)VEC3_ZERO_INIT)

typedef struct vec3 {
	union {
		struct { float x, y, z; };
		float raw[3];
	};
} vec3;

static inline vec3 vec3_add( vec3 a, vec3 b )
{
	vec3 r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

static inline vec3 vec3_sub( vec3 a, vec3 b )
{
	vec3 r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

static inline float vec3_dot( vec3 a, vec3 b )
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static inline vec3 vec3_cross( vec3 a, vec3 b )
{
	vec3 r;

	r.x = (a.y * b.z) - (a.z * b.y);
	r.y = (a.z * b.x) - (a.x * b.z);
	r.z = (a.x * b.y) - (a.y * b.x);

	return r;
}

static inline float vec3_mag2( vec3 v )
{
	return vec3_dot( v, v );
}

static inline float vec3_mag( vec3 v )
{
	return sqrtf( vec3_mag2( v ) );
}

static inline vec3 vec3_add_scalar( vec3 v, float s )
{
	vec3 r;

	r.x = v.x + s;
	r.y = v.y + s;
	r.z = v.z + s;

	return r;
}

static inline vec3 vec3_sub_scalar( vec3 v, float s )
{
	vec3 r;

	r.x = v.x - s;
	r.y = v.y - s;
	r.z = v.z - s;

	return r;
}

static inline vec3 vec3_mul( vec3 a, vec3 b )
{
	vec3 r;

	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;

	return r;
}

static inline vec3 vec3_scale( vec3 v, float s )
{
	vec3 r;

	r.x = v.x * s;
	r.y = v.y * s;
	r.z = v.z * s;

	return r;
}

static inline vec3 vec3_div( vec3 a, vec3 b )
{
	vec3 r;

	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;

	return r;
}

static inline vec3 vec3_div_scalar( vec3 v, float s )
{
	vec3 r;

	r.x = v.x / s;
	r.y = v.y / s;
	r.z = v.z / s;

	return r;
}

static inline vec3 vec3_negate( vec3 v )
{
	vec3 r;

	r.x = -v.x;
	r.y = -v.y;
	r.z = -v.z;

	return r;
}

static inline vec3 vec3_normalise( vec3 v )
{
	float mag = vec3_mag( v );

	if ( mag < VEC_EPSILON )
		return VEC3_ZERO;

	return vec3_scale( v, 1.0f / mag );
}

static inline float vec3_angle( vec3 a, vec3 b )
{
	float mag_a, mag_b, dot;

	mag_a = vec3_mag( a );
	mag_b = vec3_mag( b );

	if ( mag_a < VEC_EPSILON || mag_b < VEC_EPSILON )
		return 0.0f;

	dot = vec3_dot( a, b ) / (mag_a * mag_b);

	if ( dot > 1.0f )
		return 0.0f;
	else if ( dot < -1.0f )
		return VEC_PI;

	return acosf( dot );
}

static inline vec3 vec3_proj( vec3 a, vec3 b )
{
	return vec3_scale( b, vec3_dot( a, b ) / vec3_mag2( b ) );
}

static inline vec3 vec3_centre( vec3 a, vec3 b )
{
	return vec3_scale( vec3_add( a, b ), 0.5f );
}

static inline float vec3_dist2( vec3 a, vec3 b )
{
	float d1 = b.x - a.x;
	float d2 = b.y - a.y;
	float d3 = b.z - a.z;

	return (d1 * d1) + (d2 * d2) + (d3 * d3);
}

static inline float vec3_dist( vec3 a, vec3 b )
{
	return sqrtf( vec3_dist2( a, b ) );
}

static inline vec3 vec3_reflect( vec3 i, vec3 n )
{
	vec3 s = vec3_scale( n, 2.0f * vec3_dot( i, n ) );
	return vec3_sub( i, s );
}

#endif
