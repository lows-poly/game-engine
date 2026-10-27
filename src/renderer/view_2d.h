#ifndef ENGINE_VIEW_2D_H
#define ENGINE_VIEW_2D_H

#include <stdbool.h>

#include "primitives/mat4.h"
#include "primitives/vec2.h"

enum view_2d_scale {
	VIEW_2D_KEEP_SIZE,
	VIEW_2D_FIT
};

enum view_2d_anchor {
	VIEW_2D_TOP_LEFT,
	VIEW_2D_CENTER
};

struct camera_2d {
	vec2 pos;
	float zoom;
};

struct view_2d {
	struct camera_2d camera;
	enum view_2d_scale scale;
	enum view_2d_anchor anchor;
	int reference_width;
	int reference_height;

	int framebuffer_width;
	int framebuffer_height;

	int viewport_x;
	int viewport_y;
	int viewport_width;
	int viewport_height;

	float left;
	float top;
	float world_width;
	float world_height;

	mat4 projection;
	bool drawable;
};

#define VIEW_2D_DEFAULT {\
	.camera = {\
		.pos = { 0.0f, 0.0f },\
		.zoom = 1.0f\
	},\
	.scale = VIEW_2D_KEEP_SIZE,\
	.anchor = VIEW_2D_TOP_LEFT\
}

/**
 * view_2d_init() - Initialise with keep-size scaling, upper-left anchoring.
 * @width
 * @height
 *
 * Return: true on success.
 */
bool view_2d_init( struct view_2d *view, int width, int height );

/**
 * view_2d_update() - Recalculate the viewport and projection using framebuffer dimensions.
 * @width
 * @height
 *
 * Return: true on success.
 */
bool view_2d_update( struct view_2d *view, int width, int height );

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
                                   vec2 *world );

#endif
