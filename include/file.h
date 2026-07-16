#ifndef FILE_H
#define FILE_H

#include <stddef.h>
#include <stdbool.h>

bool file_read_binary( const char *path, void **out_data, size_t *out_size );
bool file_read_text( const char *path, char **out_data );

bool file_read_binary_rel( const char *rel_path, void **out_data, size_t *out_size );
bool file_read_text_rel( const char *rel_path, char **out_data );

void file_free( void *data );

#endif
