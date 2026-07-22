#ifndef ENGINE_IVEC3_H
#define ENGINE_IVEC3_H

#include <math.h>

#define IVEC3_ZERO_INIT { 0, 0, 0 }
#define IVEC3_ONE_INIT  { 1, 1, 1 }

#define IVEC3_ZERO      ((ivec3)IVEC3_ZERO_INIT)
#define IVEC3_ONE       ((ivec3)IVEC3_ONE_INIT)

typedef struct ivec3 {
	union {
		struct { int x, y, z; };
		int raw[3];
	};
} ivec3;

static inline ivec3 ivec3_add( ivec3 a, ivec3 b )
{
	ivec3 r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

static inline ivec3 ivec3_sub( ivec3 a, ivec3 b )
{
	ivec3 r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

static inline int ivec3_dot( ivec3 a, ivec3 b )
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static inline ivec3 ivec3_add_scalar( ivec3 v, int s )
{
	ivec3 r;

	r.x = v.x + s;
	r.y = v.y + s;
	r.z = v.z + s;

	return r;
}

static inline ivec3 ivec3_sub_scalar( ivec3 v, int s )
{
	ivec3 r;

	r.x = v.x - s;
	r.y = v.y - s;
	r.z = v.z - s;

	return r;
}

static inline ivec3 ivec3_mul( ivec3 a, ivec3 b )
{
	ivec3 r;

	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;

	return r;
}

static inline ivec3 ivec3_scale( ivec3 v, int s )
{
	ivec3 r;

	r.x = v.x * s;
	r.y = v.y * s;
	r.z = v.z * s;

	return r;
}

static inline ivec3 ivec3_div( ivec3 a, ivec3 b )
{
	ivec3 r;

	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;

	return r;
}

static inline ivec3 ivec3_div_scalar( ivec3 v, int s )
{
	ivec3 r;

	r.x = v.x / s;
	r.y = v.y / s;
	r.z = v.z / s;

	return r;
}

static inline ivec3 ivec3_negate( ivec3 v )
{
	ivec3 r;

	r.x = -v.x;
	r.y = -v.y;
	r.z = -v.z;

	return r;
}

static inline int ivec3_dist2( ivec3 a, ivec3 b )
{
	int dx, dy, dz;

	dx = b.x - a.x;
	dy = b.y - a.y;
	dz = b.z - a.z;

	return (dx * dx) + (dy * dy) + (dz * dz);
}

static inline float ivec3_dist( ivec3 a, ivec3 b )
{
	return sqrtf( (float)ivec3_dist2( a, b ) );
}

#endif
