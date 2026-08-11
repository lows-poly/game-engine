#include <errno.h>

#include "renderer.h"
#include "primitives/mat4.h"

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

/*
 * renderer_set_2d_projection()
 * @shader
 * @width
 * @height
 */
int renderer_set_2d_projection( struct shader *s, int width, int height )
{
	mat4 proj;

	if ( !s || width <= 0 || height <= 0 )
		return -EINVAL;

	/*
	 * PIXEL COORDINATES:
	 * LEFT   = 0
	 * RIGHT  = WIDTH
	 * BOTTOM = HEIGHT
	 * TOP    = 0
	 */
	mat4_ortho( 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, proj );

	return shader_set_mat4( s, "u_projection", proj );
}
