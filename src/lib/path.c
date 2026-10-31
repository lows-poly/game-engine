#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>

#if defined( __APPLE__ )
#include <mach-o/dyld.h>
#elif !defined( __linux__ )
#error "path.c: only Linux and macOS are supported"
#endif

#include "path.h"

#define ROOT_MARKER		".project-root"
#define MAX_WALK_DEPTH		32

static char project_root[PATH_MAX_LEN];

static bool get_exe_path( char *out, size_t out_size )
{
	char raw_path[PATH_MAX_LEN];
	char *resolved;
	size_t length;

#if defined( __linux__ )
	ssize_t count;
#elif defined( __APPLE__ )
	uint32_t capacity = sizeof( raw_path );
#endif

	if ( !out || out_size == 0 )
		return false;

	out[0] = '\0';

#if defined( __linux__ )
	count = readlink( "/proc/self/exe", raw_path,
	                  sizeof( raw_path ) - 1 );

	if ( count <= 0 ||
	     (size_t)count >= sizeof( raw_path ) - 1 )
		return false;

	raw_path[count] = '\0';
#elif defined( __APPLE__ )
	if ( _NSGetExecutablePath( raw_path, &capacity ) != 0 )
		return false;
#endif

	resolved = realpath( raw_path, NULL );
	if ( !resolved )
		return false;

	length = strlen( resolved );

	if ( length >= out_size ) {
		free( resolved );
		return false;
	}

	memcpy( out, resolved, length + 1 );
	free( resolved );

	return true;
}

bool path_set_root( const char *directory )
{
	struct stat info;
	char *resolved;
	size_t length;

	if ( !directory || !directory[0] )
		return false;

	resolved = realpath( directory, NULL );
	if ( !resolved )
		return false;

	length = strlen( resolved );

	if ( length >= sizeof( project_root ) || stat( resolved, &info ) != 0 ||
	     !S_ISDIR( info.st_mode ) )
	{
		free( resolved );
		return false;
	}

	memcpy( project_root, resolved, length + 1 );
	free( resolved );

	return true;
}

/*
 * Search start_dir and its parents for ROOT_MARKER.
 * Preserve the configured root if discovery fails.
 */
static bool find_project_root( const char *start_dir )
{
	struct stat info;
	char probe[PATH_MAX_LEN];
	char directory[PATH_MAX_LEN];
	char *separator;
	int written;
	int i;

	if ( !start_dir || start_dir[0] != '/' )
		return false;

	written = snprintf( directory, sizeof( directory ), "%s",
	                    start_dir );

	if ( written < 0 || (size_t)written >= sizeof( directory ) )
		return false;

	for ( i = 0; i < MAX_WALK_DEPTH; i++ ) {
		written = snprintf( probe, sizeof( probe ), "%s%s%s",
		                    directory,
		                    strcmp( directory, "/" ) == 0 ? "" : "/",
		                    ROOT_MARKER );

		if ( written < 0 || (size_t)written >= sizeof( probe ) )
			return false;

		if ( stat( probe, &info ) == 0 && S_ISREG( info.st_mode ) )
			return path_set_root( directory );

		if ( strcmp( directory, "/" ) == 0 )
			break;

		separator = strrchr( directory, '/' );
		if ( !separator )
			return false;

		if ( separator == directory )
			directory[1] = '\0';
		else
			*separator = '\0';
	}

	return false;
}

bool path_init( void )
{
	char executable[PATH_MAX_LEN];

	if ( !get_exe_path( executable, sizeof( executable ) ) )
		return false;

	return find_project_root( dirname( executable ) );
}

const char *path_root( void )
{
	return project_root[0] ? project_root : NULL;
}

bool path_resolve( const char *path, char *out, size_t out_size )
{
	int written;

	if ( !out || out_size == 0 )
		return false;

	out[0] = '\0';

	if ( !path || !path[0] )
		return false;

	if ( path[0] == '/' ) {
		written = snprintf( out, out_size, "%s", path );
	} else {
		if ( !project_root[0] )
			return false;

		written = snprintf( out, out_size, "%s%s%s",
		                    project_root,
		                    strcmp( project_root, "/" ) == 0 ? "" : "/",
		                    path );
	}

	if ( written < 0 || (size_t)written >= out_size ) {
		out[0] = '\0';
		return false;
	}

	return true;
}
