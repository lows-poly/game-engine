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

static int apply_view( struct renderer_2d *r, struct shader *shader );

void renderer_2d_destroy( struct renderer_2d *r )
{
	if ( !r )
		return;

	shader_destroy( &r->default_shader );
	shader_destroy( &r->textured_shader );
	mesh_destroy( &r->rect_mesh );
	mesh_destroy( &r->tri_mesh );

	memset( &r->view, 0, sizeof( r->view ) );

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

	if ( !view_2d_init( &r->view, width, height ) ) {
		pr_warn("FAILED TO INIT 2D VIEW\n");
		return false;
	}

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

	if ( !apply_view( r, r->shader ) ) {
		pr_warn("FAILED TO APPLY VIEW\n");
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
	if ( !r || !w || !w->handle || !r->default_shader.id )
		return;

	if ( !view_2d_update( &r->view, w->framebuffer_width, w->framebuffer_height ) ) {
		r->view.drawable = false;
		pr_warn("FAILED TO UPDATE 2D VIEW\n");
		return;
	}

	r->width = w->framebuffer_width;
	r->height = w->framebuffer_height;
}

bool renderer_2d_set_view( struct renderer_2d *r, const struct view_2d *view )
{
	struct view_2d next;

	if ( !r || !view || !r->default_shader.id )
		return false;

	next = *view;

	if ( !view_2d_update( &next, r->width, r->height ) )
		return false;

	r->view = next;
	return true;
}

bool renderer_2d_set_shader( struct renderer_2d *r, struct shader *shader )
{
	if ( !r || !shader || !shader->id || !r->default_shader.id )
		return false;

	shader_use( shader );

	if ( !shader_set_mat4( shader, "u_projection", r->view.projection ) ) {
		if ( r->shader )
			shader_use( r->shader );

		return false;
	}

	r->shader = shader;
	return true;
}

void renderer_2d_use_default_shader( struct renderer_2d *r )
{
	if ( !r )
		return;

	if ( !renderer_2d_set_shader( r, &r->default_shader ) )
		pr_warn("FAILED TO SELECT DEFAULT 2D SHADER\n");
}

void renderer_2d_draw_shape( struct renderer_2d *r, const struct shape2d *shape )
{
	struct mesh *mesh;
	mat4 model;
	float colour_arr[4];

	if ( !r || !shape || !r->shader || !r->view.drawable )
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

	if ( !apply_view( r, r->shader ) )
		return;

	if ( !shader_set_mat4( r->shader, "u_model", model ) ||
	     !shader_set_4f( r->shader, "u_colour", colour_arr ) )
	{
		return;
	}

	renderer_draw_mesh( mesh, r->shader );
}

void renderer_2d_draw_texture( struct renderer_2d *r, const struct shape2d *shape,
                               const struct texture *texture, colour tint )
{
	struct texture_region region;

	if ( !texture || !shape || !texture )
		return;

	region.x = 0;
	region.y = 0;
	region.width = texture->width;
	region.height = texture->height;

	renderer_2d_draw_texture_region( r, shape, texture, &region, tint );
}

void renderer_2d_draw_texture_region( struct renderer_2d *r, const struct shape2d *shape, const struct texture *texture,
                                      const struct texture_region *region, colour tint )
{
	mat4 model;
	float tint_arr[4];
	vec2 uv_offset;
	vec2 uv_scale;

	if ( !r || !shape || shape->type != SHAPE2D_RECTANGLE || !r->textured_shader.id ||
	     !r->rect_mesh.vao.id || !r->view.drawable )
	{
		return;
	}

	if ( !texture_region_valid( texture, region ) )
		return;

	uv_offset.x = (float)region->x / (float)texture->width;
	uv_offset.y = 1.0f - (float)region->y / (float)texture->height;

	uv_scale.x = (float)region->width / (float)texture->width;
	uv_scale.y = -(float)region->height / (float)texture->height;

	mat4_identity( model );
	mat4_translate( model, shape->pos.x, shape->pos.y, 0.0f );
	mat4_rotate_z( model, shape->rotation );
	mat4_translate( model, -shape->origin.x, -shape->origin.y, 0.0f );
	mat4_scale_3f( model, shape->scale.x, shape->scale.y, 1.0f );

	colour_to_arr( tint, tint_arr );

	if ( !apply_view( r, &r->textured_shader ) )
		return;

	if ( !shader_set_mat4( &r->textured_shader, "u_model", model ) ||
	     !shader_set_4f( &r->textured_shader, "u_tint", tint_arr ) ||
	     !shader_set_1i( &r->textured_shader, "u_texture", 0 ) ||
	     !shader_set_vec2( &r->textured_shader, "u_uv_offset", uv_offset ) ||
	     !shader_set_vec2( &r->textured_shader, "u_uv_scale", uv_scale ) )
	{
		return;
	}

	texture_bind( texture, 0 );
	renderer_draw_mesh( &r->rect_mesh, &r->textured_shader );
}

static int apply_view( struct renderer_2d *r, struct shader *shader )
{
	if ( !r || !shader || !shader->id || !r->view.drawable )
		return 0;

	shader_use( shader );

	if ( !shader_set_mat4( shader, "u_projection", r->view.projection ) )
		return 0;

	glViewport( r->view.viewport_x, r->view.viewport_y, r->view.viewport_width,
	            r->view.viewport_height );

	return 1;
}
