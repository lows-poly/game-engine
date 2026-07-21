#include "renderer.h"
#include "primitives/colourf.h"

void renderer_begin_frame( colour c )
{
	colourf cf = colour_to_float( c );

	glClearColor( cf.r, cf.g, cf.b, cf.a );
	glClear( GL_COLOR_BUFFER_BIT );
}

void renderer_enable_backface_culling( bool value, enum draw_order order )
{
	if ( value ) {
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		glFrontFace( order );
	} else {
		glDisable( GL_CULL_FACE );
	}
}

void renderer_draw_mesh( const struct shader *s, const struct mesh *m )
{
	shader_use( s );
	mesh_draw( m );
}
