#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "path.h"

#include "app/app.h"
#include "input/input.h"

#include "renderer/renderer.h"
#include "graphics/mesh.h"
#include "graphics/shader/shader.h"

#include "primitives/colour.h"
#include "primitives/colourf.h"
#include "primitives/vec3.h"

#define WINDOW_TITLE		"ENGINE"
#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
#define TARGET_FRAMERATE	60

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
	struct app app;
	struct shader shader;
	struct mesh triangle;

	if ( path_init( argv[0] ) < 0 ) {
		fprintf( stderr, "FAILED TO INIT PATH\n" );
		return EXIT_FAILURE;
	}

	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE ) ) {
		fprintf( stderr, "FAILED TO INIT APP\n" );
		return EXIT_FAILURE;
	}

	if ( shader_binit( &shader, SHADER_COLOUR ) != 0 ) {
		fprintf( stderr, "FAILED TO INIT BUILTIN SHADER\n" );
		return EXIT_FAILURE;
	}

	if ( mesh_init( &triangle, vertices, sizeof( vertices ), 3, attribs,
	                2, NULL, 0 ) != 0 ) {
		fprintf( stderr, "FAILED TO INIT MESH\n" );
		return EXIT_FAILURE;
	}

	while ( app.running ) {
		renderer_begin_frame( VINTAGE_GOLD );

		if ( key_pressed( &app.input, KEY_ESCAPE ) ) {
			app_stop( &app );
		}

		renderer_draw_mesh( &shader, &triangle );
		app_update( &app );
	}

	mesh_destroy( &triangle );
	shader_destroy( &shader );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
