#ifndef ENGINE_VEC3_H
#define ENGINE_VEC3_H

#include <math.h>

#define VEC_EPSILON	1e-8f

typedef struct vec3 {
	float x;
	float y;
	float z;
} vec3;

static inline vec3 vec3_add( vec3 a, vec3 b ) {
	vec3 r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

static inline vec3 vec3_sub( vec3 a, vec3 b ) {
	vec3 r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

static inline vec3 vec3_scale( vec3 a, float scale ) {
	vec3 r;

	r.x = a.x * scale;
	r.y = a.y * scale;
	r.z = a.z * scale;

	return r;
}

static inline float vec3_dot( vec3 a, vec3 b ) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static inline float vec3_length_sq( vec3 a ) {
	return vec3_dot( a, a );
}

static inline float vec3_length( vec3 a ) {
	return sqrtf( vec3_length_sq( a ) );
}

static inline vec3 vec3_normalise( vec3 a ) {
	float len = vec3_length( a );

	if ( len < VEC_EPSILON )
		return a;

	return vec3_scale( a, 1.0f / len );
}

static inline vec3 vec3_cross( vec3 a, vec3 b ) {
	vec3 r;

	r.x = (a.y * b.z) - (a.z * b.y);
	r.y = (a.z * b.x) - (a.x * b.z);
	r.z = (a.x * b.y) - (a.y * b.x);

	return r;
}

#endif
