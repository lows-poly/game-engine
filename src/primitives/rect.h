#ifndef ENGINE_RECT_H
#define ENGINE_RECT_H

#include <stdbool.h>
#include "mathx.h"

#include "vec2.h"

typedef struct rect {
	vec2 pos;
	vec2 size;
} rect;

static inline rect rect_make( float x, float y, float w, float h )
{
	rect r;

	r.pos = vec2_make( x, y );
	r.size = vec2_make( maxf( w, 0 ), maxf( h, 0 ) );

	return r;
}

static inline float rect_left( const rect *r )
{
	return r->pos.x;
}

static inline float rect_right( const rect *r )
{
	return r->pos.x + r->size.x;
}

static inline float rect_top( const rect *r )
{
	return r->pos.y;
}

static inline float rect_bottom( const rect *r )
{
	return r->pos.y + r->size.y;
}

static inline vec2 rect_centre( const rect *r )
{
	return vec2_make( r->pos.x + r->size.x * 0.5f, r->pos.y + r->size.y * 0.5f );
}

static inline void rect_to_vertices( const rect *r, vec2 out[4] )
{
	out[0] = vec2_make( rect_left( r ), rect_top( r ) );
	out[1] = vec2_make( rect_right( r ), rect_top( r ) );
	out[2] = vec2_make( rect_right( r ), rect_bottom( r ) );
	out[3] = vec2_make( rect_left( r ), rect_bottom( r ) );
}

static inline bool rect_contains_point( const rect *r, float x, float y )
{
	return x >= rect_left( r ) && x <= rect_right( r ) &&
	       y >= rect_top( r ) && y <= rect_bottom( r );
}

static inline bool rect_intersects( const rect *a, const rect *b )
{
	return rect_left( a ) <= rect_right( b ) &&
	       rect_right( a ) >= rect_left( b ) &&
	       rect_top( a ) <= rect_bottom( b ) &&
	       rect_bottom( a ) >= rect_top( b );
}

#endif
