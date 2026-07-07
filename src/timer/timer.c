#include <GLFW/glfw3.h>
#include "timer.h"

void timer_init( struct timer *t, double fixed_dt )
{
	t->last_time = glfwGetTime();
	t->delta_time = 0.0;
	t->accumulator = 0.0;
	t->fixed_dt = fixed_dt;
	t->time_scale = 1.0;
}

void timer_tick( struct timer *t )
{
	double now;
	double raw_dt;

	now = glfwGetTime();
	raw_dt = now - t->last_time;
	t->last_time = now;

	if ( raw_dt > 0.25 )
		raw_dt = 0.25;

	t->delta_time =	raw_dt * t->time_scale;
	t->accumulator += t->delta_time;
}

int timer_should_step( struct timer *t )
{
	return t->accumulator >= t->fixed_dt;
}

void timer_step( struct timer *t )
{
	t->accumulator -= t->fixed_dt;
}
