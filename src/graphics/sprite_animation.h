#ifndef ENGINE_SPRITE_ANIMATION_H
#define ENGINE_SPRITE_ANIMATION_H

#include <stdbool.h>
#include <stddef.h>

#include "graphics/texture/texture.h"

/**
 * struct sprite_animation - Shared animation definition.
 * @frames: Borrowed array of source regions, in playback order.
 * @frame_count: Number of regions in the array.
 * @frame_dt: Duration of each frame, in seconds.
 * @loop: Restart after the last frame.
 */
struct sprite_animation {
	const struct texture_region *frames;
	size_t frame_count;
	double frame_dt;
	bool loop;
};

/**
 * struct sprite_animator - Playback state for one sprite.
 * @animation: Borrowed definition.
 * @frame: Current frame index.
 * @elapsed: Time accumulated within the current frame.
 * @playing: Whether playback advances.
 */
struct sprite_animator {
	const struct sprite_animation *animation;
	size_t frame;
	double elapsed;
	bool playing;
};

struct sprite;

bool sprite_animator_init( struct sprite_animator *animator, struct sprite *sprite,
                           const struct sprite_animation *animation );

bool sprite_animator_update( struct sprite_animator *animator, struct sprite *sprite,
                             double dt );

#endif
