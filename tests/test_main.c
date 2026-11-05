#include <stdio.h>
#include <stdlib.h>

#include "test.h"

int main( void )
{
	if ( !test_rect() )
		return EXIT_FAILURE;

	puts( "All tests passed." );
	return EXIT_SUCCESS;
}
