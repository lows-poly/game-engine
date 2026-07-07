#ifndef DARRAY_H
#define DARRAY_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define darray(T) struct { \
	T *data; \
	size_t count; \
	size_t capacity; \
}

#define darray_alloc( arr ) \
	( { \
		int _err = 0; \
		\
		(arr) = malloc( sizeof( *(arr) ) ); \
		if ( !(arr) ) { \
			_err = -ENOMEM; \
		} else { \
			(arr)->data = NULL; \
			(arr)->count = 0; \
			(arr)->capacity = 0; \
		} \
		_err; \
	} )

#define darray_free( arr ) \
	do { \
		if ( arr ) { \
			free( (arr)->data ); \
			free( arr ); \
			(arr) = NULL; \
		} \
	} while (0)

#define darray_reserve( arr, n ) \
	( { \
		int _err = 0; \
		\
		if ( (arr)->capacity < (size_t)(n) ) { \
			size_t new_cap_; \
			void *new_data_; \
			\
			if ( (arr)->capacity > 0 ) \
				new_cap_ = (arr)->capacity; \
			else \
				new_cap_ = 4; \
			\
			while ( new_cap_ < (size_t)(n) ) \
				new_cap_ *= 2; \
			\
			new_data_ = realloc( (arr)->data, \
			                      new_cap_ * sizeof( *(arr)->data ) ); \
			\
			if ( !new_data_ ) { \
				_err = -ENOMEM; \
			} else { \
				(arr)->data = new_data_; \
				(arr)->capacity = new_cap_; \
			} \
		} \
		_err; \
	} )

#define darray_push( arr, src ) \
	( { \
		int _err = darray_reserve( (arr), (arr)->count + 1 ); \
		\
		if ( !_err ) \
			(arr)->data[(arr)->count++] = (src); \
		_err; \
	} )

#define darray_pop( arr, out ) \
	( { \
		int _err = 0; \
		\
		if ( (arr)->count == 0 ) { \
			_err = -ENOENT; \
		} else { \
			(arr)->count--; \
			*(out) = (arr)->data[(arr)->count]; \
		} \
		_err; \
	} )

#define darray_remove( arr, index ) \
	( { \
		int _err = 0; \
		\
		if ( (size_t)(index) >= (arr)->count ) { \
			_err = -ERANGE; \
		} else { \
			memmove( &(arr)->data[index], &(arr)->data[(index) + 1], \
			         ( (arr)->count - (index) - 1 ) * sizeof( *(arr)->data ) ); \
			(arr)->count--; \
		} \
		_err; \
	} )

#define darray_get( arr, index, out ) \
	( { \
		int _err = 0; \
		\
		if ( (size_t)(index) >= (arr)->count ) { \
			_err = -ERANGE; \
		} else { \
			*(out) = (arr)->data[index]; \
		} \
		_err; \
	} )

#define darray_clear( arr ) \
	( (arr)->count = 0 )

#define darray_at( arr, index ) \
	( (arr)->data[index] )

#define darray_empty( arr ) \
	( (arr)->count == 0 )

#define darray_size( arr ) \
	( (arr)->count )

#endif
