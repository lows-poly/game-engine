#include <math.h>

#include "log.h"

#include "view_2d.h"

static bool config_valid( const struct view_2d *view )
{
	if ( view->reference_width <= 0 || view->reference_height <= 0 )
		return false;

	if ( !isfinite( view->camera.pos.x ) || !isfinite( view->camera.pos.y ) ||
	     !isfinite( view->camera.zoom ) || view->camera.zoom <= 0.0f )
	{
		return false;
	}

	if ( view->scale != VIEW_2D_KEEP_SIZE && view->scale != VIEW_2D_FIT )
		return false;

	if ( view->anchor != VIEW_2D_TOP_LEFT && view->anchor != VIEW_2D_CENTER )
		return false;

	return true;
}

static double fit_viewport( struct view_2d *view )
{
	double scale_x;
	double scale_y;
	double scale;
	int width = view->framebuffer_width;
	int height = view->framebuffer_height;

	scale_x = (double)width / view->reference_width;
	scale_y = (double)height / view->reference_height;
	scale = fmin( scale_x, scale_y );

	view->viewport_width = (int)floor( view->reference_width * scale );
	view->viewport_height = (int)floor( view->reference_height * scale );

	if ( view->viewport_width < 1 )
		view->viewport_width = 1;

	if ( view->viewport_height < 1 )
		view->viewport_height = 1;

	if ( view->anchor == VIEW_2D_CENTER ) {
		view->viewport_x = (width - view->viewport_width) / 2;
		view->viewport_y = (height - view->viewport_height) / 2;
	} else {
		view->viewport_x = 0;
		view->viewport_y = height - view->viewport_height;
	}

	scale_x = (double)view->viewport_width / view->reference_width;
	scale_y = (double)view->viewport_height / view->reference_height;

	return fmin( scale_x, scale_y );
}

static bool update_projection( struct view_2d *view, double scale )
{
	float right;
	float bottom;
	int col;
	int row;

	view->world_width = (float)(view->viewport_width / scale / (double)view->camera.zoom);
	view->world_height = (float)(view->viewport_height / scale / (double)view->camera.zoom);

	if ( !isfinite( view->world_width ) || !isfinite( view->world_height ) ||
	     view->world_width <= 0.0f || view->world_height <= 0.0f )
	{
		return false;
	}

	view->left = view->camera.pos.x;
	view->top = view->camera.pos.y;

	if ( view->anchor == VIEW_2D_CENTER ) {
		view->left -= view->world_width * 0.5f;
		view->top -= view->world_height * 0.5f;
	}

	right = view->left + view->world_width;
	bottom = view->top + view->world_height;

	if ( !isfinite( view->left ) || !isfinite( view->top ) ||
	     !isfinite( right ) || !isfinite( bottom ) || right <= view->left ||
	     bottom <= view->top )
	{
		return false;
	}

	mat4_ortho( view->left, right, bottom, view->top, -1.0f, 1.0f, view->projection );

	for ( col = 0; col < 4; col++ ) {
		for ( row = 0; row < 4; row++ ) {
			if ( !isfinite( view->projection[col][row] ) )
				return false;
		}
	}

	return true;
}

/**
 * view_2d_init() - Initialise with keep-size scaling, upper-left anchoring.
 * @width
 * @height
 *
 * Return: true on success.
 */
bool view_2d_init( struct view_2d *view, int width, int height )
{
	struct view_2d next = VIEW_2D_DEFAULT;

	if ( !view || width <= 0 || height <= 0 ) {
		pr_warn("INVALID ARGUMENTS\n");
		return false;
	}

	next.reference_width = width;
	next.reference_height = height;

	if ( !view_2d_update( &next, width, height ) ) {
		pr_warn("VIEW UPDATE FAILED\n");
		return false;
	}

	*view = next;
	return true;
}

/**
 * view_2d_update() - Recalculate the viewport and projection using framebuffer dimensions.
 * @width
 * @height
 *
 * Return: true on success.
 */
bool view_2d_update( struct view_2d *view, int width, int height )
{
	struct view_2d next;
	double scale = 1.0;

	if ( !view || width < 0 || height < 0 )
		return false;

	if ( !config_valid( view ) )
		return false;

	next = *view;
	next.framebuffer_width = width;
	next.framebuffer_height = height;
	next.viewport_x = 0;
	next.viewport_y = 0;
	next.viewport_width = width;
	next.viewport_height = height;
	next.drawable = false;

	if ( !width || !height ) {
		*view = next;
		return true;
	}

	if ( next.scale == VIEW_2D_FIT )
		scale = fit_viewport( &next );

	if ( !update_projection( &next, scale ) )
		return false;

	next.drawable = true;
	*view = next;

	return true;
}

/**
 * view_2d_framebuffer_to_world() - Convert top-left framebuffer coordinates into world coordinates.
 * @width
 * @height
 *
 * Return false for points outside the viewport, including letterbox bars,
 * or when drawing is suspended. Output remains unchanged on failure.
 *
 * GLFW cursor coordinates must first be converted from window units
 * into framebuffer pixels.
 */
bool view_2d_framebuffer_to_world( const struct view_2d *view, float x, float y,
                                   vec2 *world )
{
	vec2 result;
	float local_x;
	float local_y;
	int viewport_top;

	if ( !view || !world || !view->drawable || !isfinite( x ) || !isfinite( y ) )
		return false;

	/* Convert the OpenGL viewport origin to a top-left origin. */
	viewport_top = view->framebuffer_height - view->viewport_y - view->viewport_height;

	local_x = x - (float)view->viewport_x;
	local_y = y - (float)viewport_top;

	/* Right and bottom edges are outside the viewport. */
	if ( local_x < 0.0f || local_y < 0.0f ||
	     local_x >= (float)view->viewport_width ||
	     local_y >= (float)view->viewport_height )
	{
		return false;
	}

	result.x = view->left + local_x / (float)view->viewport_width * view->world_width;
	result.y = view->top + local_y / (float)view->viewport_height * view->world_height;

	if ( !isfinite( result.x ) || !isfinite( result.y ) )
		return false;

	*world = result;
	return true;
}
