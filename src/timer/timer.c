#include <math.h>
#include <string.h>

#include <GLFW/glfw3.h>

#include "timer.h"

#define TIMER_MAX_FRAME_DT	0.25

bool timer_init( struct timer *t, double fixed_dt )
{
	if ( !t || !isfinite( fixed_dt ) || fixed_dt <= 0.0 )
		return false;

	memset( t, 0, sizeof( *t ) );

	t->last_time = glfwGetTime();
	t->fixed_dt = fixed_dt;
	t->time_scale = 1.0;
	t->max_frame_dt = TIMER_MAX_FRAME_DT;
	t->init = true;

	return true;
}

bool timer_set_fixed_dt( struct timer *t, double fixed_dt )
{
	if ( !t || !t->init || !isfinite( fixed_dt ) || fixed_dt <= 0.0 )
		return false;

	t->fixed_dt = fixed_dt;
	t->accumulator = fmod( t->accumulator, fixed_dt );

	return true;
}

bool timer_set_time_scale( struct timer *t, double time_scale )
{
	if ( !t || !t->init || !isfinite( time_scale ) || time_scale < 0.0 )
		return false;

	t->time_scale = time_scale;

	return true;
}

void timer_tick( struct timer *t )
{
	double now;
	double raw_dt;

	if ( !t || !t->init )
		return;

	now = glfwGetTime();
	raw_dt = now - t->last_time;
	t->last_time = now;

	if ( !isfinite( raw_dt ) || raw_dt < 0.0 )
		raw_dt = 0.0;

	if ( raw_dt > t->max_frame_dt )
		raw_dt = t->max_frame_dt;

	t->delta_time = raw_dt * t->time_scale;
	t->accumulator += t->delta_time;
}

bool timer_step( struct timer *t )
{
	if ( !t || !t->init || t->accumulator < t->fixed_dt )
		return false;

	t->accumulator -= t->fixed_dt;

	return true;
}

double timer_alpha( const struct timer *t )
{
	if ( !t || !t->init || t->fixed_dt <= 0.0 )
		return 0.0;

	return t->accumulator / t->fixed_dt;
}
