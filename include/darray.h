#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <string.h>

#define darray(T) struct { \
	T *data; \
	size_t count; \
	size_t capacity \
}

#define darray_alloc( arr ) \
	do { \
		(arr) = malloc( sizeof( *(arr) ) ); \
		if ( arr ) { \
			(arr)->data = NULL; \
			(arr)->count = 0; \
			(arr)->capacity = 0; \
		} \
	} while (0)

#define darray_free( arr ) \
	do { \
		if ( arr ) { \
			free( (arr)->data ); \
			free( arr ); \
			(arr) = NULL; \
		} \
	} while (0)

#define darray_reserve( arr, n ) \
	do { \
		if ( (arr)->capacity < (size_t)(n) ) { \
			size_t new_cap_; \
			\
			if ( (arr)->capacity > 0 ) \
				new_cap_ = (arr)->capacity; \
			else \
				new_cap_ = 4; \
			\
			while ( new_cap_ < (size_t)(n) ) \
				new_cap_ *= 2; \
			\
			void *new_data_ = realloc( (arr)->data, \
			                           new_cap_ * sizeof( *(arr)->data ) ) \
			\
			if ( new_data_ ) {\
				(arr)->data = new_data_; \
				(arr)->capacity = new_cap_; \
			} \
		} \
	} while (0)

#define darray_push( arr, src ) \
	do { \
		darray_reserve( (arr), (arr)->count + 1 ); \
		(arr)->data[(arr)->count++] = (src); \
	} while (0)

#define darray_pop( arr ) \
	( (arr)->data[--(arr)->count] );

#define darray_remove( arr, index ) \
	do { \
		memmove( &(arr)->data[index], &(arr)->data[(index) + 1] ), \
		         (( (arr)->count - (index) - 1 )) * sizeof( *(arr)->data ));\
		(arr)->count--; \
	} while (0)

#define darray_clear( arr ) \
	( (arr)->count = 0 )

#define darray_at( arr, index ) \
	( (arr)->data[index] )

#define darray_empty( arr ) \
	( (arr)->count == 0 )

#define darray_size( arr ) \
	( (arr)->count )

#endif
