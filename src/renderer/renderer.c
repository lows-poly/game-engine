#include "renderer.h"

void renderer_clear( GLbitfield mask )
{
	glClear( mask );
}

void renderer_clear_colour( colour c )
{
	colourf cf = colour_to_float( c );
	glClearColor( cf.r, cf.g, cf.b, cf.a );
}

void renderer_clear_colourf( colourf cf )
{
	glClearColor( cf.r, cf.g, cf.b, cf.a );
}
