#ifndef ENGINE_VEC4_H
#define ENGINE_VEC4_H

#include <math.h>

#include "vec3.h"

typedef struct vec4 {
	float x;
	float y;
	float z;
	float w;
} vec4;

static inline void vec4_zero( vec4 *a ) {
	a->x = 0.0f;
	a->y = 0.0f;
	a->z = 0.0f;
	a->w = 0.0f;
}

static inline vec4 vec4_add( vec4 a, vec4 b ) {
	vec4 r;
 
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	r.w = a.w + b.w;
 
	return r;
}
 
static inline vec4 vec4_sub( vec4 a, vec4 b ) {
	vec4 r;
 
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	r.w = a.w - b.w;
 
	return r;
}
 
static inline vec4 vec4_scale( vec4 a, float scale ) {
	vec4 r;
 
	r.x = a.x * scale;
	r.y = a.y * scale;
	r.z = a.z * scale;
	r.w = a.w * scale;
 
	return r;
}
 
static inline float vec4_dot( vec4 a, vec4 b ) {
	return ( a.x * b.x ) + ( a.y * b.y ) + ( a.z * b.z ) + ( a.w * b.w );
}
 
static inline vec4 vec3_to_vec4( vec3 a, float w ) {
	vec4 r;
 
	r.x = a.x;
	r.y = a.y;
	r.z = a.z;
	r.w = w;
 
	return r;
}
 
static inline vec3 vec4_to_vec3( vec4 a ) {
	vec3 r;
 
	r.x = a.x;
	r.y = a.y;
	r.z = a.z;
 
	return r;
}

#endif
