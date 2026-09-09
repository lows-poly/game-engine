#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>

#include "primitives/vec2.h"

#define G		1.9f
#define G_SOFT		7.4f
#define G_MAX_SPEED	150.0f
#define G_MIN_MASS	1.5f

/*
 * gravity_force() - Newtonian force vector body i exerts on body j.
 * @pos_i: position of the attracting body
 * @pos_j: position of the attracted body
 * @mass_i: mass of the attracting body
 *
 * Returns a force vector pointing from pos_j toward pos_i with a softened
 * denominator to avoid singularities at close range.
 */
static inline vec2 gravity_force( vec2 pos_i, vec2 pos_j, float mass_i )
{
	vec2 delta;
	float dist_sq, dist;
	float force;

	delta = vec2_sub( pos_i, pos_j );

	dist_sq = vec2_mag_sq( delta ) + (G_SOFT * G_SOFT);
	dist = sqrtf( dist_sq );

	force = G * mass_i / dist_sq;

	return vec2_scale( delta, force / dist );
}

/*
 * gravity_apply() - Apply N-body gravitational acceleration to velocities.
 * @pos: position array
 * @vel: velocity array
 * @mass: mass array
 * @count: number of bodies
 * @dt: delta time
 *
 * Only stars with mass >= G_MIN_MASS are treated as attractors.
 * O(count * attractor_count) instead of O(count^2).
 */
static inline void gravity_apply( vec2 *pos, vec2 *vel, const float *mass,
                                  int count, float dt )
{
	int attractors[count > 0 ? count : 1];
	int attractor_count;
	int i, j, k;
	vec2 accel;
	float speed;

	if ( !pos || !vel || !mass || count <= 0 )
		return;

	attractor_count = 0;

	for ( j = 0; j < count; j++ ) {
		if ( mass[j] >= G_MIN_MASS )
			attractors[attractor_count++] = j;
	}

	printf("ATTRACTING BODIES: %d\n", attractor_count);

	for ( i = 0; i < count; i++ ) {
		accel = VEC2_ZERO;

		for ( k = 0; k < attractor_count; k++ ) {
			j = attractors[k];

			if ( i == j )
				continue;

			accel = vec2_add( accel, gravity_force( pos[j], pos[i], mass[j] ) );
		}

		vel[i] = vec2_add( vel[i], vec2_scale( accel, dt ) );

		speed = vec2_mag( vel[i] );

		if ( speed > G_MAX_SPEED )
			vel[i] = vec2_scale( vel[i], G_MAX_SPEED / speed );
	}
}

#endif
