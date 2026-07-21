#include <stdio.h>

#include "primitives/vec3.h"

int main( void )
{
	vec3 v = { 1.0f, 0.0f, 0.0f };

	for ( int i = 0; i < 3; i++ )
		printf("%.1f\n", v.raw[i]);

	return 0;
}
