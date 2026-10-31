#ifndef ENGINE_PATH_H
#define ENGINE_PATH_H

#include <stdbool.h>
#include <stddef.h>

#define PATH_MAX_LEN	4096

/**
 * Discover the resource root by searching for .project-root above
 * the executable directory.
 *
 * Return false if discovery fails. An existing root is preserved.
 */
bool path_init( void );

bool path_set_root( const char *directory );

/**
 * Return the selected absolute root, or NULL if none is configured.
 * The returned string is owned by this module.
 */
const char *path_root( void );

/**
 * Copy an absolute path, or join a relative path to the resource root.
 * Relative paths require a configured root.
 *
 * out_size includes space for the terminating null character.
 * Input and output must not overlap.
 * On failure, clear out when it is non-NULL and out_size is nonzero.
 */
bool path_resolve( const char *path, char *out, size_t out_size );

#endif
