#ifndef ENGINE_FILE_H
#define ENGINE_FILE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Read an entire binary file.
 *
 * Relative paths use the working directory.
 * Both output pointers are required.
 *
 * On success, return an allocated buffer and its byte count.
 * Empty files succeed with an allocated buffer and a zero byte count.
 * Release the buffer with file_free().
 *
 * On failure, reset supplied outputs to NULL and zero.
 */
bool file_read_binary( const char *path, void **out_data,
                       size_t *out_size );

/**
 * Read an entire file and append a terminating null character.
 *
 * Relative paths use the working directory.
 * out_data is required and is reset to NULL on failure.
 * Empty files succeed with an allocated empty string.
 * Release the buffer with file_free().
 */
bool file_read_text( const char *path, char **out_data );

/**
 * Resolve through path_resolve(), then read the file.
 *
 * Relative paths use the configured resource root.
 * Absolute paths are accepted without a configured root.
 * Output and ownership rules match the corresponding functions above.
 */
bool file_read_binary_rel( const char *path, void **out_data,
                           size_t *out_size );
bool file_read_text_rel( const char *path, char **out_data );
void file_free( void *data );

#endif
