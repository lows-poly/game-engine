#include "file.h"
#include <stdio.h>
#include <stdlib.h>

#include "path.h"

int file_read_binary( const char *path, void **out_data, size_t *out_size )
{
	FILE *fp;
	long size;
	void *buf;

	fp = fopen( path, "rb" );
	if ( fp == NULL )
		return -1;

	if ( fseek( fp, 0, SEEK_END ) < 0 ) {
		fclose( fp );
		return -1;
	}

	size = ftell( fp );
	if ( size < 0 ) {
		fclose( fp );
		return -1;
	}

	if ( fseek( fp, 0, SEEK_SET ) < 0 ) {
		fclose( fp );
		return -1;
	}

	buf = malloc( (size_t) size );
	if ( buf == NULL ) {
		fclose( fp );
		return -1;
	}

	if ( fread( buf, 1, (size_t) size, fp ) != (size_t) size ) {
		free( buf );
		fclose( fp );
		return -1;
	}

	fclose( fp );

	*out_data = buf;
	*out_size = (size_t) size;

	return 0;
}

int file_read_binary_rel( const char *rel_path, void **out_data, size_t *out_size )
{
	char full_path[PATH_MAX_LEN];

	if ( path_resolve( rel_path, full_path, sizeof( full_path ) ) < 0 )
		return -1;

	return file_read_binary( full_path, out_data, out_size );
}

int file_read_text_rel( const char *rel_path, char **out_data )
{
	char full_path[PATH_MAX_LEN];

	if ( path_resolve( rel_path, full_path, sizeof( full_path ) ) < 0 )
		return -1;

	return file_read_text( full_path, out_data );
}

int file_read_text( const char *path, char **out_data )
{
	void *data;
	size_t size;
	char *buf;

	if ( file_read_binary( path, &data, &size ) < 0 )
		return -1;

	buf = realloc( data, size + 1 );
	if ( buf == NULL ) {
		free( data );
		return -1;
	}

	buf[size] = '\0';
	*out_data = buf;

	return 0;
}

void file_free( void *data )
{
	free( data );
}
