#ifndef ENGINE_IVEC2_H
#define ENGINE_IVEC2_H

#include <math.h>

#define IVEC2_ZERO_INIT { 0, 0 }
#define IVEC2_ONE_INIT  { 1, 1 }

#define IVEC2_ZERO      ((ivec2)IVEC2_ZERO_INIT)
#define IVEC2_ONE       ((ivec2)IVEC2_ONE_INIT)

typedef struct ivec2 {
	union {
		struct { int x, y; };
		int raw[2];
	};
} ivec2;

static inline ivec2 ivec2_add( ivec2 a, ivec2 b )
{
	ivec2 r;

	r.x = a.x + b.x;
	r.y = a.y + b.y;

	return r;
}

static inline ivec2 ivec2_sub( ivec2 a, ivec2 b )
{
	ivec2 r;

	r.x = a.x - b.x;
	r.y = a.y - b.y;

	return r;
}

static inline int ivec2_dot( ivec2 a, ivec2 b )
{
	return (a.x * b.x) + (a.y * b.y);
}

static inline int ivec2_cross( ivec2 a, ivec2 b )
{
	return (a.x * b.y) - (a.y * b.x);
}

static inline ivec2 ivec2_add_scalar( ivec2 v, int s )
{
	ivec2 r;

	r.x = v.x + s;
	r.y = v.y + s;

	return r;
}

static inline ivec2 ivec2_sub_scalar( ivec2 v, int s )
{
	ivec2 r;

	r.x = v.x - s;
	r.y = v.y - s;

	return r;
}

static inline ivec2 ivec2_mul( ivec2 a, ivec2 b )
{
	ivec2 r;

	r.x = a.x * b.x;
	r.y = a.y * b.y;

	return r;
}

static inline ivec2 ivec2_scale( ivec2 v, int s )
{
	ivec2 r;

	r.x = v.x * s;
	r.y = v.y * s;

	return r;
}

static inline ivec2 ivec2_div( ivec2 a, ivec2 b )
{
	ivec2 r;

	r.x = a.x / b.x;
	r.y = a.y / b.y;

	return r;
}

static inline ivec2 ivec2_div_scalar( ivec2 v, int s )
{
	ivec2 r;

	r.x = v.x / s;
	r.y = v.y / s;

	return r;
}

static inline ivec2 ivec2_negate( ivec2 v )
{
	ivec2 r;

	r.x = -v.x;
	r.y = -v.y;

	return r;
}

static inline int ivec2_dist2( ivec2 a, ivec2 b )
{
	int dx, dy;

	dx = b.x - a.x;
	dy = b.y - a.y;

	return (dx * dx) + (dy * dy);
}

static inline float ivec2_dist( ivec2 a, ivec2 b )
{
	return sqrtf( (float)ivec2_dist2( a, b ) );
}

#endif
