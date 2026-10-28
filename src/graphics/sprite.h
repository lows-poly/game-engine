#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#include <stdbool.h>

#include "graphics/texture/texture.h"
#include "primitives/shape2d.h"

/**
 * struct sprite - A textured rectangle.
 * @texture
 * @region: Source rectangle in image pixels, measured from top left.
 * @shape: Display position, size, origin, rotation, and colour tint.
 */
struct sprite {
	const struct texture *texture;
	struct texture_region region;
	struct shape2d shape;
};

bool sprite_init( struct sprite *sprite, const struct texture *texture,
                  const struct texture_region *region );

#endif
