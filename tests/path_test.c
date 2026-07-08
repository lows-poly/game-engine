#include <stdio.h>

#include "path.h"

int main( int argc, char *argv[] )
{
	printf("TESTING PATH\n");
	char asset_path[PATH_MAX_LEN];

	if ( path_init( argv[0] ) < 0 )
		return 1;

	printf( "project root: %s\n", path_root( ) );

	if ( path_resolve( "assets/linux_preview.png", asset_path, sizeof( asset_path ) ) < 0 )
		return 1;

	printf( "loading: %s\n", asset_path );

	return 0;
}
