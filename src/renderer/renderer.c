#include "renderer.h"

/* 
 * renderer_begin_frame() - Call glClear and glClearColor
 * @c: colour primitive
 *
 * Convert colour primitive to float
 */
void renderer_begin_frame( colour c )
{
	glClearColor( c.r, c.g, c.b, c.a );
	glClear( GL_COLOR_BUFFER_BIT );
}

/* 
 * renderer_enable_backface_culling() - Enable backface culling
 * @value: bool
 * @order: front face draw order
 */
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

/*
 * renderer_draw_mesh() - Use shader and draw mesh
 * @s: shader
 * @m: mesh
 */
void renderer_draw_mesh( const struct mesh *m, const struct shader *s )
{
	shader_use( s );
	mesh_draw( m );
}
