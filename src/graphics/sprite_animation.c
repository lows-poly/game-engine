#include <math.h>

#include "sprite_animation.h"
#include "sprite.h"

bool sprite_animator_init( struct sprite_animator *animator, struct sprite *sprite,
                           const struct sprite_animation *animation )
{
	struct sprite_animator next = { 0 };
	size_t i;

	if ( !animator || !sprite || !animation )
		return false;

	if ( sprite->shape.type != SHAPE2D_RECTANGLE )
		return false;

	if ( !animation->frames || !animation->frame_count )
		return false;

	if ( !isfinite( animation->frame_dt ) || animation->frame_dt <= 0.0 )
		return false;

	for ( i = 0; i < animation->frame_count; i++ ) {
		if ( !texture_region_valid( sprite->texture, &animation->frames[i] ) )
			return false;
	}

	next.animation = animation;
	next.frame = 0;
	next.elapsed = 0.0;
	next.playing = true;

	sprite->region = animation->frames[0];
	*animator = next;

	return true;
}

bool sprite_animator_update( struct sprite_animator *animator, struct sprite *sprite,
                             double dt )
{
	const struct sprite_animation *animation;
	struct sprite_animator next;

	if ( !animator || !sprite || !isfinite( dt ) || dt < 0.0 )
		return false;

	animation = animator->animation;

	if ( !animation || !animation->frames || !animation->frame_count )
		return false;

	if ( !isfinite( animation->frame_dt ) || animation->frame_dt <= 0.0 )
		return false;

	if ( animator->frame >= animation->frame_count ||
	     !isfinite( animator->elapsed ) || animator->elapsed < 0.0 )
	{
		return false;
	}

	if ( !animator->playing || dt == 0.0 )
		return true;

	next = *animator;
	next.elapsed += dt;

	if ( !isfinite( next.elapsed ) )
		return false;

	while ( next.elapsed >= animation->frame_dt ) {
		next.elapsed -= animation->frame_dt;

		if ( next.frame < animation->frame_count - 1 ) {
			next.frame++;
		} else if ( animation->loop ) {
			next.frame = 0;
		} else {
			next.playing = false;
			next.elapsed = 0.0;
			break;
		}
	}

	if ( !texture_region_valid( sprite->texture, &animation->frames[next.frame] ) )
		return false;

	sprite->region = animation->frames[next.frame];
	*animator = next;

	return true;
}
