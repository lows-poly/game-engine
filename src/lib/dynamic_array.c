#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

#define DEFAULT_CAPACITY    4

/*
 * arr_init() - Initialise a dynamic array.
 * @arr:  Array to initialise
 * @size: Size in bytes of a single element
 * @cap:  Initial capacity
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_init( struct darray *arr, size_t size, size_t cap )
{
	if ( !arr || size == 0 )
		return 0;

	memset( arr, 0, sizeof( *arr  ) );
	arr->data_size = size;

	if ( cap == 0 )
		return 1;

	arr->data = malloc( cap * size );
	if ( !arr->data )
		return 0;

	arr->cap = cap;
	return 1;
}

/*
 * arr_destroy() - Free array storage and reset to an empty state.
 * @arr: Array to destroy
 */
void arr_destroy( struct darray *arr )
{
	if ( !arr )
		return;

	free( arr->data );
	arr->data = NULL;
	arr->count = 0;
	arr->cap = 0;
}

/*
 * arr_reseve() - Ensure capacity for at least @new_cap elements.
 * @arr:     Array to reserve
 * @new_cap: Minimum capacity
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_reserve( struct darray *arr, size_t new_cap )
{
	void *tmp;
	size_t grown_cap;

	if ( !arr )
		return 0;

	if ( new_cap <= arr->cap )
		return 1;

	if ( arr->cap > 0 )
		grown_cap = arr->cap * 2;
	else
		grown_cap = DEFAULT_CAPACITY;

	tmp = realloc( arr->data, grown_cap * arr->data_size );
	if ( !tmp )
		return 0;

	arr->data = tmp;
	arr->cap = grown_cap;

	return 1;
}

/*
 * arr_push() - Append a copy of *data to the end of array.
 * @arr:  Array to push onto
 * @data: Pointer to data to copy in
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_push( struct darray *arr, void *data )
{
	unsigned char *dst;

	if ( !arr || !data )
		return 0;

	if ( arr->count == arr->cap ) {
		if ( !arr_reserve( arr, arr->count + 1 ) )
			return 0;
	}

	dst = (unsigned char *)arr->data + (arr->count * arr->data_size);
	memcpy( dst, data, arr->data_size );
	arr->count++;

	return 1;
}

/*
 * arr_pop() - Remove the last element of the array and copy it out.
 * @arr: Array to pop
 * @out: Destination buffer of the removed data, NULL to discard
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_pop( struct darray *arr, void *out )
{
	unsigned char *src;

	if ( !arr || arr_empty( arr ) )
		return 0;

	arr->count--;
	src = (unsigned char*)arr->data + (arr->count * arr->data_size);
	if ( out )
		memcpy( out, src, arr->data_size );

	return 1;
}

/*
 * arr_get() - Copy the data @idx and outout to @out.
 * @arr: Array to retrieve an element from
 * @idx: Index of the data
 * @out: Destination buffer of the data
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_get( struct darray *arr, size_t idx, void *out )
{
	const unsigned char *src;

	if ( !arr || !out || idx >= arr->count )
		return 0;

	src = (unsigned char *)arr->data + (idx * arr->data_size);
	memcpy( out, src, arr->data_size );

	return 1;
}

/*
 * arr_set() - Overwrite the data at a specific index.
 * @arr:  Array of the element to overwrite
 * @idx:  Index of the data
 * @data: Pointer to the new data
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_set( struct darray *arr, size_t idx, void *data )
{
	unsigned char *dst;

	if ( !arr || !data || idx >= arr->count )
		return 0;

	dst = (unsigned char *)arr->data + (idx * arr->data_size);
	memcpy( dst, data, arr->data_size );

	return 1;
}

/*
 * arr_at() - Direct pointer to the element at @idx, no copy.
 * @arr: Array to index into
 * @idx: index of the data
 *
 * Return: pointer to the data, or NULL if idx is out of range.
 */
void *arr_at( struct darray *arr, size_t idx )
{
	if ( !arr || idx >= arr->count )
		return NULL;

	return (unsigned char *)arr->data + (idx * arr->data_size);
}

/*
 * arr_empty() - Check whether the array holds no data.
 * @arr: Array to check
 *
 * Return:
 * 1 - Empty
 * 0 - Otherwise
 */
int arr_empty( const struct darray *arr )
{
	if ( !arr )
		return 1;
	
	return arr->count == 0;
}
