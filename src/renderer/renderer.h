#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <glad/glad.h>

#include "primitives/colour.h"
#include "primitives/colourf.h"

void renderer_clear( GLbitfield mask );
void renderer_clear_colour( colour c );
void renderer_clear_colourf( colourf c );

#endif
