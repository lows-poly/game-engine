#include <stddef.h>
#include <string.h>

#include "renderer_2d.h"
#include "renderer.h"
#include "primitives/mat4.h"
#include "log.h"

struct textured_vertex {
	vec2 pos;
	vec2 uv;
};

static const struct textured_vertex UNIT_QUAD[4] = {
	{ { 0.0f, 0.0f }, { 0.0f, 0.0f } },
	{ { 1.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { 0.0f, 1.0f }, { 0.0f, 1.0f } }
};

static const unsigned int QUAD_INDICES[6] = { 0, 1, 2, 2, 3, 0 };

static const vec2 UNIT_TRI[3] = {
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 0.5f, 0.0f }
};

static const struct vertex_attrib TEXTURED_QUAD_ATTRIBS[2] = {
	{
		.index = 0,
		.size = 2,
		.type = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride = sizeof( struct textured_vertex ),
		.offset = offsetof( struct textured_vertex, pos )
	},
	{
		.index = 1,
		.size = 2,
		.type = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride = sizeof( struct textured_vertex ),
		.offset = offsetof( struct textured_vertex, uv )
	}
};

static int set_projection( struct renderer_2d *r, int width, int height );

void renderer_2d_destroy( struct renderer_2d *r )
{
	if ( !r )
		return;

	shader_destroy( &r->default_shader );
	shader_destroy( &r->textured_shader );
	mesh_destroy( &r->rect_mesh );
	mesh_destroy( &r->tri_mesh );

	r->shader = NULL;
	r->width = 0;
	r->height = 0;
}

/*
 * renderer_2d_init() - Init shader 2d preset and set projection.
 * @renderer_2d struct
 * @width
 * @height
 *
 * return: false on failure.
 */
bool renderer_2d_init( struct renderer_2d *r, int width, int height )
{
	struct mesh_desc quad_desc;

	if ( !r || width <= 0 || height <= 0 ) {
		pr_warn("INVALID ARGUMENTS\n");
		return false;
	}

	memset( r, 0, sizeof( *r ) );

	quad_desc.vertices = UNIT_QUAD;
	quad_desc.vertex_size = sizeof( UNIT_QUAD );
	quad_desc.attribs = TEXTURED_QUAD_ATTRIBS;
	quad_desc.attrib_count = 2;
	quad_desc.indices = QUAD_INDICES;
	quad_desc.vertex_count = 4;
	quad_desc.index_count = 6;

	if ( !mesh_init( &r->rect_mesh, &quad_desc, DRAW_STATIC ) ) {
		pr_warn("FAILED TO INIT MESH QUAD\n");
		goto FAIL;
	}

	if ( !mesh_init_tri( &r->tri_mesh, UNIT_TRI, DRAW_STATIC ) ) {
		pr_warn("FAILED TO INIT MESH TRI\n");
		goto FAIL;
	}

	if ( shader_init_preset( &r->default_shader, SHADER_BASIC_2D ) != 0 ) {
		pr_warn("FAILED TO INIT DEFAULT SHADER PRESET\n");
		goto FAIL;
	}

	if ( shader_init_preset( &r->textured_shader, SHADER_TEXTURED_2D ) != 0 ) {
		pr_warn("FAILED TO INIT TEXTURED SHADER PRESET\n");
		goto FAIL;
	}

	r->shader = &r->default_shader;
	r->width = width;
	r->height = height;

	if ( !set_projection( r, width, height ) ) {
		pr_warn("PROJECTION FAILED\n");
		renderer_2d_destroy( r );
		return false;
	}

	return true;
FAIL:
	renderer_2d_destroy( r );
	return false;
}

void renderer_2d_update( struct renderer_2d *r, struct window *w )
{
	int width;
	int height;

	if ( !r || !w )
		return;

	if ( !window_take_resize( w, &width, &height ) )
		return;

	if ( width <= 0 || height <= 0 )
		return;

	(void)set_projection( r, width, height );
}

bool renderer_2d_set_shader( struct renderer_2d *r, struct shader *shader )
{
	struct shader *prev;

	if ( !r || !shader )
		return false;

	prev = r->shader;
	r->shader = shader;

	if ( !set_projection( r, r->width, r->height ) ) {
		r->shader = prev;
		return false;
	}

	return true;
}

void renderer_2d_use_default_shader( struct renderer_2d *r )
{
	if ( !r )
		return;

	r->shader = &r->default_shader;
	if ( !set_projection( r, r->width, r->height ) )
		pr_warn("2D PROJECTION ENCOUNTERED AN ERROR WHILE SWITCHING TO DEFAULT SHADER\n");
}

void renderer_2d_draw_shape( struct renderer_2d *r, const struct shape2d *shape )
{
	struct mesh *mesh;
	mat4 model;
	float colour_arr[4];

	if ( !r || !shape || !r->shader )
		return;

	switch ( shape->type ) {
	case SHAPE2D_RECTANGLE:
		mesh = &r->rect_mesh;
		break;
	case SHAPE2D_TRIANGLE:
		mesh = &r->tri_mesh;
		break;
	default:
		return;
	}

	colour_to_arr( shape->colour, colour_arr );

	mat4_identity( model );
	mat4_translate( model, shape->pos.x, shape->pos.y, 0.0f );
	mat4_rotate_z( model, shape->rotation );
	mat4_translate( model, -shape->origin.x, -shape->origin.y, 0.0f );
	mat4_scale_3f( model, shape->scale.x, shape->scale.y, 1.0f );

	shader_use( r->shader );

	if ( !shader_set_mat4( r->shader, "u_model", model ) ||
	     !shader_set_4f( r->shader, "u_colour", colour_arr ) )
		return;

	renderer_draw_mesh( mesh, r->shader );
}

void renderer_2d_draw_texture( struct renderer_2d *r, const struct shape2d *shape,
                               const struct texture *texture, colour tint )
{
	mat4 projection;
	mat4 model;
	float tint_arr[4];

	if ( !r || !shape || !texture || !texture->id ||
	     shape->type != SHAPE2D_RECTANGLE ||
	     !r->textured_shader.id || !r->rect_mesh.vao.id )
	{
		return;
	}

	mat4_ortho( 0.0f, (float)r->width, (float)r->height,
	            0.0f, -1.0f, 1.0f, projection );

	mat4_identity( model );
	mat4_translate( model, shape->pos.x, shape->pos.y, 0.0f );
	mat4_rotate_z( model, shape->rotation );
	mat4_translate( model, -shape->origin.x, -shape->origin.y, 0.0f );
	mat4_scale_3f( model, shape->scale.x, shape->scale.y, 1.0f );

	colour_to_arr( tint, tint_arr );

	shader_use( &r->textured_shader );

	if ( !shader_set_mat4( &r->textured_shader, "u_projection", projection ) ||
	     !shader_set_mat4( &r->textured_shader, "u_model", model ) ||
	     !shader_set_4f( &r->textured_shader, "u_tint", tint_arr ) ||
	     !shader_set_1i( &r->textured_shader, "u_texture", 0 ) )
	{
		return;
	}

	texture_bind( texture, 0 );
	renderer_draw_mesh( &r->rect_mesh, &r->textured_shader );
}

static int set_projection( struct renderer_2d *r, int width, int height )
{
	mat4 proj;

	if ( !r || !r->shader )
		return 0;

	if ( width <= 0 || height <= 0 ) {
		pr_warn("FAILED TO SET 2D PROJECTION: INVALID ARGUMENTS\n");
		return 0;
	}

	/*
	 * PIXEL COORDINATES:
	 * LEFT   = 0
	 * RIGHT  = WIDTH
	 * TOP    = 0
	 * BOTTOM = HEIGHT
	 */
	mat4_ortho( 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, proj );

	shader_use( r->shader );

	if ( !shader_set_mat4( r->shader, "u_projection", proj ) ) {
		pr_warn("FAILED TO SET 2D PROJECTION UNIFORM\n");
		return 0;
	}

	r->width = width;
	r->height = height;

	return 1;
}
