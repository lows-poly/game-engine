#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <sys/stat.h>

#if defined( __linux__ )
#elif defined( __APPLE__ )
#include <mach-o/dyld.h>
#else
#error "path.c: UNSUPPORTED PLATFORM -- ONLY LINUX AND MACOS ARE SUPPORTED"
#endif

#include "path.h"

#define ROOT_MARKER		".project-root"
#define MAX_WALK_DEPTH		32

static char project_root[PATH_MAX_LEN];

static int get_exe_path( char *exe_path, size_t exe_path_size )
{
#if defined( __linux__ )
	ssize_t len;
 
	len = readlink( "/proc/self/exe", exe_path, exe_path_size - 1 );
 
	if ( len <= 0 )
		return -1;
 
	exe_path[ len ] = '\0';
 
	return 0;
 
#elif defined( __APPLE__ )
	char     raw_path[PATH_MAX_LEN];
	uint32_t raw_size = sizeof( raw_path );
 
	if ( _NSGetExecutablePath( raw_path, &raw_size ) != 0 )
		return -1;
 
	/* _NSGetExecutablePath may return a path containing symlinks
	 * or relative components -- resolve it to a canonical path. */
	if ( realpath( raw_path, exe_path ) == NULL )
		return -1;
 
	( void ) exe_path_size;
 
	return 0;
#endif
}


static int file_exists( const char *path )
{
	struct stat st;

	return stat( path, &st ) == 0;
}

/*
 * Walk upward from start_dir looking for ROOT_MARKER. On success,
 * project_root is filled in and 0 is returned. On failure, -1 is
 * returned and project_root is left untouched.
 */
static int find_project_root( const char *start_dir )
{
	char probe[PATH_MAX_LEN];
	char dir[PATH_MAX_LEN];
	int  i;

	snprintf( dir, sizeof( dir ), "%s", start_dir );

	for ( i = 0; i < MAX_WALK_DEPTH; i++ ) {
		int n;

		n = snprintf( probe, sizeof( probe ), "%s/%s", dir, ROOT_MARKER );

		if ( n < 0 || ( size_t ) n >= sizeof( probe ) )
			return -1;

		if ( file_exists( probe ) ) {
			snprintf( project_root, sizeof( project_root ), "%s", dir );
			return 0;
		}

		if ( strcmp( dir, "/" ) == 0 )
			break;

		snprintf( probe, sizeof( probe ), "%s", dir );
		snprintf( dir, sizeof( dir ), "%s", dirname( probe ) );
	}

	return -1;
}

int path_init( const char *argv0 )
{
	char exe_path[PATH_MAX_LEN];
	char resolved[PATH_MAX_LEN];
	char dir_buf[PATH_MAX_LEN];
	ssize_t len;

	len = readlink( "/proc/self/exe", exe_path, sizeof( exe_path ) - 1 );

	if ( len > 0 ) {
		exe_path[len] = '\0';
	} else {
		if ( realpath( argv0, resolved ) == NULL ) {
			fprintf( stderr, "path_init: cannot resolve '%s': %s\n",
				 argv0, strerror( errno ) );
			return -1;
		}
		snprintf( exe_path, sizeof( exe_path ), "%s", resolved );
	}

	snprintf( dir_buf, sizeof( dir_buf ), "%s", exe_path );

	if ( find_project_root( dirname( dir_buf ) ) < 0 ) {
		fprintf( stderr, "path_init: could not locate '%s' above '%s'\n",
			 ROOT_MARKER, dirname( exe_path ) );
		return -1;
	}

	return 0;
}

const char *path_root( void )
{
	return project_root;
}

int path_resolve( const char *rel, char *out, size_t out_size )
{
	int n;

	if ( project_root[0] == '\0' ) {
		fprintf( stderr, "path_resolve: path_init() not called\n" );
		return -1;
	}

	n = snprintf( out, out_size, "%s/%s", project_root, rel );

	if ( n < 0 || ( size_t ) n >= out_size ) {
		fprintf( stderr, "path_resolve: truncated path for '%s'\n", rel );
		return -1;
	}

	return 0;
}
