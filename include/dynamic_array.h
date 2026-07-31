/*
 * Generic dynamic array of fixed-size elements.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>

struct darray {
	void	*data;
	size_t	data_size;
	size_t	count;
	size_t	cap;
};

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
int arr_init( struct darray *arr, size_t size, size_t cap );

/*
 * arr_destroy() - Free array storage and reset to an empty state.
 * @arr: Array to destroy
 */
void arr_destroy( struct darray *arr );

/*
 * arr_reseve() - Ensure capacity for at least @new_cap elements.
 * @arr:     Array to reserve
 * @new_cap: Minimum capacity
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_reserve( struct darray *arr, size_t new_cap );

/*
 * arr_push() - Append a copy of *data to the end of array.
 * @arr:  Array to push onto
 * @data: Pointer to data to copy in
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_push( struct darray *arr, void *data );

/*
 * arr_pop() - Remove the last element of the array and copy it out.
 * @arr: Array to pop
 * @out: Destination buffer of the removed data, NULL to discard
 *
 * Return:
 * 1 - Success
 * 0 - Failure
 */
int arr_pop( struct darray *arr, void *out );

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
int arr_get( struct darray *arr, size_t idx, void *out );

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
int arr_set( struct darray *arr, size_t idx, void *data );

/*
 * arr_at() - Direct pointer to the element at @idx, no copy.
 * @arr: Array to index into
 * @idx: index of the data
 *
 * Return: pointer to the data, or NULL if idx is out of range.
 */
void *arr_at( struct darray *arr, size_t idx );
 
/*
 * arr_empty() - Check whether the array holds no data.
 * @arr: Array to check
 *
 * Return:
 * 1 - Empty
 * 0 - Otherwise
 */
int arr_empty( const struct darray *arr );

#endif
