#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "path.h"

static bool file_read_all( const char *path, void **out_data,
                          size_t *out_size )
{
	FILE *fp;
	unsigned char *buffer = NULL;
	long file_size;
	size_t size;
	bool success = false;

	if ( out_data )
		*out_data = NULL;

	if ( out_size )
		*out_size = 0;

	if ( !path || !path[0] || !out_data || !out_size )
		return false;

	fp = fopen( path, "rb" );
	if ( !fp )
		return false;

	if ( fseek( fp, 0, SEEK_END ) != 0 )
		goto CLOSE;

	file_size = ftell( fp );
	if ( file_size < 0 )
		goto CLOSE;

	if ( (uintmax_t)file_size > (uintmax_t)( SIZE_MAX - 1 ) )
		goto CLOSE;

	size = (size_t)file_size;

	if ( fseek( fp, 0, SEEK_SET ) != 0 )
		goto CLOSE;

	buffer = malloc( size + 1 );
	if ( !buffer )
		goto CLOSE;

	if ( fread( buffer, 1, size, fp ) != size || ferror( fp ) )
		goto CLOSE;

	buffer[size] = '\0';
	success = true;

CLOSE:
	if ( fclose( fp ) != 0 )
		success = false;

	if ( !success ) {
		free( buffer );
		return false;
	}

	*out_data = buffer;
	*out_size = size;

	return true;
}

bool file_read_binary( const char *path, void **out_data,
                       size_t *out_size )
{
	return file_read_all( path, out_data, out_size );
}

bool file_read_binary_rel( const char *path, void **out_data,
                           size_t *out_size )
{
	char full_path[PATH_MAX_LEN];

	if ( out_data )
		*out_data = NULL;

	if ( out_size )
		*out_size = 0;

	if ( !out_data || !out_size )
		return false;

	if ( !path_resolve( path, full_path, sizeof( full_path ) ) )
		return false;

	return file_read_binary( full_path, out_data, out_size );
}

bool file_read_text_rel( const char *path, char **out_data )
{
	char full_path[PATH_MAX_LEN];

	if ( !out_data )
		return false;

	*out_data = NULL;

	if ( !path_resolve( path, full_path, sizeof( full_path ) ) )
		return false;

	return file_read_text( full_path, out_data );
}

bool file_read_text( const char *path, char **out_data )
{
	void *data;
	size_t size;

	if ( !out_data )
		return false;

	*out_data = NULL;

	if ( !file_read_all( path, &data, &size ) )
		return false;

	*out_data = data;

	return true;
}

void file_free( void *data )
{
	free( data );
}
