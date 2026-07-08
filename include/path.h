#ifndef PATH_H
#define PATH_H

#include <stddef.h>

#define PATH_MAX	4096

int path_init( const char *argv0 );
const char *path_root( void );
int path_resolve( const char *rel, char *out, size_t out_size );

#endif
