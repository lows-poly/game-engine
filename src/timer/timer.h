#ifndef ENGINE_TIMER_H
#define ENGINE_TIMER_H

struct timer {
	double last_time;
	double delta_time;
	double accumulator;
	double fixed_dt;
	double time_scale;
};

int timer_init( struct timer *t, double fixed_dt );
void timer_set_dt( struct timer *t, double fixed_dt );
void timer_tick( struct timer *t );
void timer_step( struct timer *t );

#endif
