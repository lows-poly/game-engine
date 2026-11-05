#include "test.h"

#include "primitives/rect.h"

bool test_rect( void )
{
	rect r;
	rect overlap;
	rect separate;
	vec2 centre;

	r = rect_make( 10.0f, 20.0f, 30.0f, 40.0f );

	TEST_ASSERT( rect_left( &r ) == 10.0f );
	TEST_ASSERT( rect_right( &r ) == 40.0f );
	TEST_ASSERT( rect_top( &r ) == 20.0f );
	TEST_ASSERT( rect_bottom( &r ) == 60.0f );

	centre = rect_centre( &r );
	TEST_ASSERT( centre.x == 25.0f );
	TEST_ASSERT( centre.y == 40.0f );

	TEST_ASSERT( rect_contains_point( &r, 20.0f, 30.0f ) );
	TEST_ASSERT( !rect_contains_point( &r, 50.0f, 30.0f ) );

	overlap = rect_make( 30.0f, 40.0f, 20.0f, 20.0f );
	separate = rect_make( 100.0f, 100.0f, 20.0f, 20.0f );

	TEST_ASSERT( rect_intersects( &r, &overlap ) );
	TEST_ASSERT( !rect_intersects( &r, &separate ) );

	return true;
}
