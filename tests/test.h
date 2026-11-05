#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdbool.h>

#define TEST_ASSERT( condition )                                         \
	do {                                                             \
		if ( !( condition ) ) {                                  \
			fprintf( stderr, "%s:%d: assertion failed %s\n", \
			         __FILE__, __LINE__, #condition );       \
			return false;                                    \
		}                                                        \
	} while ( 0 )

bool test_rect( void );

#endif
