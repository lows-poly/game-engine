#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <GLFW/glfw3.h>

#include "timer.h"

/*
 * _warn() - Print an internal timer subsystem warning
 * @msg: Warning mesage
 */
static void _warn( const char *msg )
{
	fprintf( stderr, "TIMER: %s\n", msg );
}

int timer_init( struct timer *t, double fixed_dt )
{
	assert( t != NULL );

	if ( fixed_dt <= 0.0 ) {
		_warn("INVALID PARAMETER: fixed_dt");
		return 0;
	}

	t->last_time = glfwGetTime();
	t->delta_time = 0.0;
	t->accumulator = 0.0;
	t->fixed_dt = fixed_dt;
	t->time_scale = 1.0;

	return 1;
}

void timer_set_dt( struct timer *t, double fixed_dt )
{
	assert( fixed_dt > 0 );

	if ( !t )
		return;

	t->fixed_dt = fixed_dt;
}

void timer_tick( struct timer *t )
{
	double now, raw_dt;

	assert( t != NULL );

	now = glfwGetTime();
	raw_dt = now - t->last_time;
	t->last_time = now;

	if ( raw_dt > 0.25 )
		raw_dt = 0.25;

	t->delta_time =	raw_dt * t->time_scale;
	t->accumulator += t->delta_time;
}

void timer_step( struct timer *t )
{
	assert( t != NULL );

	if ( t->accumulator < t->fixed_dt )
		return;

	t->accumulator -= t->fixed_dt;
}
