#include "shader.h"
#include "lib/path.h"

int shader_create( struct shader *s, const char *vert_path, const char *frag_path )
{
	char *vert_src = vert_path;
	char *frag_src = frag_path;

	if ( !vert_code || !frag_code )i {
		goto clean_shader_src;
		return 0;
	}



	clean_shader_src:
		free( vert_src );
		free( frag_src );

}
