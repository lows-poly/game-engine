#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <glad/glad.h>
#include <stdbool.h>

#include "graphics/shader/shader.h"
#include "graphics/mesh.h"
#include "primitives/colour.h"

enum draw_order {
	DRAW_ORDER_CW = GL_CW,
	DRAW_ORDER_CCW = GL_CCW
};

void renderer_begin_frame( colour c );
void renderer_enable_backface_culling( bool value, enum draw_order order );
void renderer_draw_mesh( const struct shader *s, const struct mesh *m );

#endif
