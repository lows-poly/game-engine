#ifndef ENGINE_RECT_H
#define ENGINE_RECT_H

#include <stdbool.h>

#include "vec2.h"

typedef struct rect {
	union {
		struct { vec2 pos; vec2 size; };
		struct { float x, y, w, h; };
	};
} rect;

static inline rect rect_make( float x, float y, float w, float h )
{
	rect r;

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	return r;
}

static inline rect rect_make_vec2( vec2 pos, vec2 size )
{
	rect r;

	r.pos = pos;
	r.size = size;

	return r;
}

static inline float rect_left( const rect *r )
{
	return r->x;
}

static inline float rect_right( const rect *r )
{
	return r->x + r->w;
}

static inline float rect_top( const rect *r )
{
	return r->y + r->h;
}

static inline float rect_bottom( const rect *r )
{
	return r->y;
}

static inline void rect_to_vertices( const rect *r, vec2 *out )
{
	out[0].x = rect_left( r );  out[0].y = rect_bottom( r );
	out[1].x = rect_right( r ); out[1].y = rect_bottom( r );
	out[2].x = rect_right( r ); out[2].y = rect_top( r );
	out[3].x = rect_left( r );  out[3].y = rect_top( r );
}

static inline vec2 rect_centre( const rect *r )
{
	vec2 c;

	c.x = r->x + (r->w * 0.5f);
	c.y = r->y + (r->h * 0.5f);

	return c;
}

static inline bool rect_contains_point( const rect *r, float x, float y )
{
	bool contains_point;

	contains_point = true;

	if ( x < rect_left( r ) || x > rect_right( r ) )
		contains_point = false;

	if ( y < rect_bottom( r ) || y > rect_top( r ) )
		contains_point = false;

	return contains_point;
}

static inline bool rect_intersects( const rect *a, const rect *b )
{
	bool intersects;

	intersects = true;

	if ( rect_left( a ) > rect_right( b ) || rect_right( a ) < rect_left( b ) )
		intersects = false;

	if ( rect_bottom( a ) > rect_top( b ) || rect_top( a ) < rect_bottom( b ) )
		intersects = false;

	return intersects;
}

#endif
