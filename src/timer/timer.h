#ifndef ENGINE_TIMER_H
#define ENGINE_TIMER_H

#include <stdbool.h>

struct timer {
	double last_time;
	double delta_time;
	double accumulator;
	double fixed_dt;
	double time_scale;
	double max_frame_dt;
	bool init;
};

bool timer_init( struct timer *t, double fixed_dt );
bool timer_set_fixed_dt( struct timer *t, double fixed_dt );
bool timer_set_time_scale( struct timer *t, double time_scale );
void timer_tick( struct timer *t );
bool timer_step( struct timer *t );
double timer_alpha( const struct timer *t );

#endif
