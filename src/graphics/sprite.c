#include "sprite.h"

bool sprite_init( struct sprite *sprite, const struct texture *texture,
                  const struct texture_region *region )
{
	struct sprite next = { 0 };

	if ( !sprite || !texture || !region )
		return false;

	if ( !texture_region_valid( texture, region ) )
		return false;

	if ( !shape2d_create( &next.shape, SHAPE2D_RECTANGLE, 0.0f, 0.0f,
	                      (float)region->width, (float)region->height ) )
	{
		return false;
	}

	next.shape.colour = WHITE;
	next.texture = texture;
	next.region = *region;

	*sprite = next;
	return true;
}
