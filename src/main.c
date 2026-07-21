#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <err.h>

#include "window/window.h"
#include "input/input.h"
#include "timer/timer.h"

#include "graphics/buffers/vertex_array.h"
#include "graphics/mesh.h"
#include "graphics/shader/shader.h"

#include "renderer/renderer.h"

#include "primitives/colour.h"
#include "primitives/colourf.h"
#include "primitives/vec3.h"

#include "path.h"

#define WINDOW_TITLE		"ENGINE"
#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
#define TARGET_FRAMERATE	1.0 / 60.0

struct vertex {
	vec3 position;
	colourf colour;
};

static const struct vertex vertices[] = {
	//  x      y     z      |  r     g     b     a
	{ { 0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ {-0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
};

static const struct vertex_attrib attribs[] = {
	{
		.index = 0,
		.size = 3,
		.type = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride = sizeof( struct vertex ),
		.offset = offsetof( struct vertex, position )
	},
	{
		.index = 1,
		.size = 4,
		.type = GL_FLOAT,
		.normalised = GL_FALSE,
		.stride = sizeof( struct vertex ),
		.offset = offsetof( struct vertex, colour )
	}
};

int main( int argc, char *argv[] )
{
	struct window_state window;
	struct input_state input;
	struct timer tm;

	struct shader shader;
	struct mesh triangle;

	int err_;

	if ( path_init( argv[0] ) < 0 )
		err(EXIT_FAILURE, "FAILED TO INIT PATH");

	err_ = window_init( &window, WINDOW_WIDTH, WINDOW_HEIGHT,
	                    WINDOW_TITLE );
	if ( err_ )
		err(EXIT_FAILURE, "FAILED TO INIT WINDOW");

	input_init( &input, window.handle );
	timer_init( &tm, TARGET_FRAMERATE );

	err_ = shader_create( &shader, "src/graphics/shader/glsl/vert_default.glsl",
	                      "src/graphics/shader/glsl/frag_default.glsl" );
	
	if ( err_ )
		err(EXIT_FAILURE, "FAILED TO CREATE SHADER");

	err_ = mesh_create( &triangle, vertices, sizeof( vertices ), 3, attribs,
	                    2, NULL, 0 );
	if ( err_ )
		err(EXIT_FAILURE, "FAILED TO CREATE MESH");

	while ( !window_should_close( &window ) ) {
		window_poll_events( &window );

		renderer_clear_colour( BLACK );
		renderer_clear( GL_COLOR_BUFFER_BIT );

		timer_tick( &tm );

		while ( timer_should_step( &tm ) ) {
			timer_step( &tm );
		}

		if ( input_key_pressed( &input, KEY_ESCAPE ) )
			window_set_should_close( &window, true );

		shader_use( &shader );
		mesh_draw( &triangle );

		window_swap_buffers( &window );
		input_update( &input );
	}

	mesh_destroy( &triangle );
	shader_destroy( &shader );
	input_destroy( &input, window.handle );
	window_destroy( &window );

	return EXIT_SUCCESS;
}
