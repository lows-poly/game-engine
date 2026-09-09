#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "app/app.h"
#include "window/window.h"
#include "input/input.h"

#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"

#include "primitives/shape2d.h"
#include "primitives/colour.h"

#include "physics.h"
#include "mathx.h"
#include "log.h"

#define WINDOW_TITLE		"ENGINE"
#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600

#define STAR_MAX		8086

#define GALAXY_ARMS		3
#define GALAXY_TURNS		1.06f
#define GALAXY_ARM_SPREAD	0.95f
#define GALAXY_RADIUS_BIAS	2.85f
#define ORBIT_SWIRL		0.02f

#define STAR_MERGE_MIN_SCALE	1.5f
#define STAR_RADIUS_FACTOR	0.55
#define STAR_MERGE_GROWTH	0.9f

struct star_field {
	struct mesh *mesh;
	mat4	*model;
	colour	*colour;
	vec2	*pos;
	vec2	*vel;
	float	*mass;
	float	*scale;
	float	*rot;
	float	*rot_speed;
	float	*phase;
	float	*speed;
	int count;
	int cap;
	int bounds_w;
	int bounds_h;
};

static colour star_rand_colour( void )
{
	int n;
	static const colour palette[] = {
		{ 0.61f, 0.71f, 1.00f, 1.0f },
		{ 0.61f, 0.71f, 1.00f, 1.0f },
		{ 0.79f, 0.85f, 1.00f, 1.0f },
		{ 0.79f, 0.85f, 1.00f, 1.0f },
		{ 1.00f, 1.00f, 1.00f, 1.0f },
		{ 1.00f, 1.00f, 1.00f, 1.0f },
		{ 1.00f, 0.96f, 0.84f, 1.0f },
		{ 1.00f, 0.85f, 0.55f, 1.0f },
		{ 1.00f, 0.65f, 0.40f, 1.0f }
	};

	n = (int)( sizeof( palette ) / sizeof( palette[0] ) );

	return palette[rand() % n];
}

static float star_rand_mass( float scale )
{
	float mass;

	mass = scale * randf( 0.8f, 1.0f );

	/* RANDOM CHANCE OF A SMALL, DENSE, HIGH MASS STAR (something like
	 * neutron star) */
	// if ( randf( 0.0f, 1.0f ) < 0.001f )
	// 	mass *= randf( 100.0f, 200.0f );

	return mass;
}
static float star_rand_scale( void )
{
	float scale;

	scale = randf( 0.5f, 1.5f );

	if ( randf( 0.0f, 1.0f ) < 0.009f )
		scale = randf( 3.0f, 5.0f );

	return scale;
}

static int stars_init( struct star_field *sf, struct mesh *mesh, int count,
                       int bounds_w, int bounds_h );
static void stars_place_spiral( struct star_field *sf, int i );
static void stars_place_random( struct star_field *sf, int i );
static void stars_update( struct star_field *sf, float dt, float elapsed );
static void stars_draw( struct star_field *sf, struct shader *shader );
static void stars_merge( struct star_field *sf, int i, int j );
static void stars_swap_remove( struct star_field *sf, int i );
static void stars_resolve_collisions( struct star_field *sf );
static void stars_destroy( struct star_field *sf );

int main( int argc, char *argv[] )
{
	struct app app;
	struct renderer_2d renderer;
	struct star_field stars;

	float time_, dt;
	int i;

	time_ = 0;
	srand(time(0));

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	/* RENDERER SETUP */
	if ( renderer_2d_init( &renderer, app.win.width, app.win.height ) != 0 )
		return EXIT_FAILURE;

	/* STARS SETUP */
	if ( !stars_init( &stars, &renderer.rect_mesh, STAR_MAX, WINDOW_WIDTH,
	                  WINDOW_HEIGHT ) ) {
		renderer_2d_destroy( &renderer );
		app_shutdown( &app );
		return EXIT_FAILURE;
	}

	for ( i = 0; i < STAR_MAX - 1; i++ ) {
		if ( randf( 0.0f, 1.0f ) <= 0.25f )
			/* SCREEN RANDOM */
			stars_place_random( &stars, i );
		else
			/* GALAXY */
			stars_place_spiral( &stars, i );
	}

	while ( app.running ) {
		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		dt = (float)app.time.delta_time;
		time_ += dt;
		
		stars_update( &stars, dt, time_ );
		stars_draw( &stars, renderer.shader );

		app_update( &app );
	}

	stars_destroy( &stars );
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}

/*
 * HELPERS
 */

static int stars_init( struct star_field *sf, struct mesh *mesh, int count,
                        int bounds_w, int bounds_h )
{
	if ( !sf || !mesh || count <= 0 || count > STAR_MAX )
		return 0;

	memset( sf, 0, sizeof( *sf ) );

	sf->model = malloc( sizeof( *sf->model ) * count );
	sf->colour = malloc( sizeof( *sf->colour ) * count );
	sf->pos = malloc( sizeof( *sf->pos ) * count );
	sf->vel = malloc( sizeof( *sf->vel ) * count );
	sf->mass = malloc( sizeof( *sf->mass ) * count );
	sf->scale = malloc( sizeof( *sf->scale ) * count );
	sf->rot = malloc( sizeof( *sf->rot ) * count );
	sf->rot_speed = malloc( sizeof( *sf->rot_speed ) * count );
	sf->phase = malloc( sizeof( *sf->phase ) * count );
	sf->speed = malloc( sizeof( *sf->speed ) * count );

	if ( !sf->model || !sf->colour || !sf->pos || !sf->vel || !sf->mass ||
	     !sf->scale || !sf->rot || !sf->rot_speed || !sf->phase || !sf->speed ) {
		stars_destroy( sf );
		return 0;
	}

	sf->count = count;
	sf->mesh = mesh;
	sf->bounds_w = bounds_w;
	sf->bounds_h = bounds_h;

	return 1;
}

static void stars_place_spiral( struct star_field *sf, int i )
{
	vec2 center;
	float max_radius;
	float t, radius, angle;
	int arm;
	float arm_angle;
	vec2 to_star;
	vec2 tangent, inward;

	if ( !sf || i < 0 || i >= sf->count )
		return;

	center = vec2_make( (float)sf->bounds_w * 0.5f, (float)sf->bounds_h * 0.5f );
	max_radius = fminf( (float)sf->bounds_w, (float)sf->bounds_h ) * 0.45f;

	t = powf( randf( 0.0f, 1.0f ), GALAXY_RADIUS_BIAS );
	radius = t * max_radius;

	arm = rand() % GALAXY_ARMS;
	arm_angle = ( (float)TWO_PI / (float)GALAXY_ARMS ) * (float)arm;

	angle = arm_angle + t * GALAXY_TURNS * (float)TWO_PI;

	angle += randf( -GALAXY_ARM_SPREAD, GALAXY_ARM_SPREAD );
	radius += randf( -8.0f, 8.0f );

	sf->pos[i].x = center.x + cosf( angle ) * radius;
	sf->pos[i].y = center.y + sinf( angle ) * radius;

	sf->scale[i] = star_rand_scale();
	sf->mass[i] = star_rand_mass( sf->scale[i] );

	to_star = vec2_sub( sf->pos[i], center );

	if ( vec2_mag( to_star ) > VEC_EPSILON ) {
		tangent = vec2_normalise( vec2_make( -to_star.y, to_star.x ) );
		inward = vec2_normalise( vec2_scale( to_star, -1.0f ) );

		sf->vel[i] = vec2_add( vec2_scale( tangent, radius * -ORBIT_SWIRL ),
		                       vec2_scale( inward, radius * 0.008f ) );
	} else {
		sf->vel[i] = VEC2_ZERO;
	}

	sf->rot[i] = randf( 0.0f, (float)TWO_PI );
	sf->rot_speed[i] = randf( -0.3f, 0.3f );

	sf->colour[i] = star_rand_colour();

	sf->phase[i] = randf( 0.0f, (float)TWO_PI );
	sf->speed[i] = randf( 4.0f, 8.0f );
}

static void stars_place_random( struct star_field *sf, int i )
{
	if ( !sf || i < 0 || i >= sf->count )
		return;

	sf->pos[i].x = randf( 0.0f, (float)sf->bounds_w );
	sf->pos[i].y = randf( 0.0f, (float)sf->bounds_h );

	sf->scale[i] = star_rand_scale();
	sf->mass[i] = star_rand_mass( sf->scale[i] );

	sf->vel[i] = VEC2_ZERO;
	sf->rot[i] = randf( 0.0f, (float)TWO_PI );
	sf->rot_speed[i] = randf( -0.3f, 0.3f );

	sf->colour[i] = star_rand_colour();

	sf->phase[i] = randf( 0.0f, (float)TWO_PI );
	sf->speed[i] = randf( 4.0f, 8.0f );
}

static void stars_update( struct star_field *sf, float dt, float elapsed )
{
	int i;
	float t;

	if ( !sf )
		return;

	gravity_apply( sf->pos, sf->vel, sf->mass, sf->count, dt );

	for ( i = 0; i < sf->count; i++ ) {
		sf->pos[i].x = sf->pos[i].x + sf->vel[i].x * dt;
		sf->pos[i].y = sf->pos[i].y + sf->vel[i].y * dt;

		sf->rot[i] += sf->rot_speed[i] * dt;
	}

	stars_resolve_collisions( sf );

	for ( i = 0; i < sf->count; i++ ) {
		if ( sf->scale[i] >= 5.0f ) {
			sf->colour[i].a = 1.0f;
		} else {
			t = elapsed * sf->speed[i] + sf->phase[i];
			sf->colour[i].a = 0.5f + 0.25f * sinf( t );
		}

		mat4_identity( sf->model[i] );
		mat4_translate( sf->model[i], sf->pos[i].x, sf->pos[i].y, 0.0f );
		mat4_rotate_z( sf->model[i], sf->rot[i] );
		mat4_translate( sf->model[i], -0.5f * sf->scale[i], -0.5f * sf->scale[i],
		                0.0f );
		mat4_scale_3f( sf->model[i], sf->scale[i], sf->scale[i], 1.0f );
	}
}

static void stars_draw( struct star_field *sf, struct shader *shader )
{
	float colour_arr[4];
	int i;

	if ( !sf || !shader )
		return;

	shader_use( shader );

	for ( i = 0; i < sf->count; i++ ) {
		colour_to_arr( sf->colour[i], colour_arr );

		shader_set_mat4( shader, "u_model", sf->model[i] );
		shader_set_4f( shader, "u_colour", colour_arr );
		renderer_draw_mesh( sf->mesh, shader );
	}
}

static void stars_merge( struct star_field *sf, int i, int j )
{
	float m_i, m_j, m_new;
	float area_new;

	m_i = sf->mass[i];
	m_j = sf->mass[j];
	m_new = m_i + m_j;

	sf->pos[i] = vec2_scale( vec2_add( vec2_scale( sf->pos[i], m_i ),
	                                   vec2_scale( sf->pos[j], m_j ) ),
	                        1.0f / m_new );

	sf->vel[i] = vec2_scale( vec2_add( vec2_scale( sf->vel[i], m_i ),
	                                   vec2_scale( sf->vel[j], m_j ) ),
	                        1.0f / m_new );

	area_new = sf->scale[i] * sf->scale[i] + sf->scale[j] * sf->scale[j];
	sf->scale[i] = sqrtf( area_new ) * STAR_MERGE_GROWTH;

	sf->mass[i] = m_new;

	if ( m_j > m_i ) {
		sf->colour[i] = sf->colour[j];
		sf->rot[i] = sf->rot[j];
		sf->rot_speed[i] = sf->rot_speed[j];
	}
}

static void stars_swap_remove( struct star_field *sf, int i )
{
	int last;

	last = sf->count - 1;

	if ( i != last ) {
		sf->pos[i]       = sf->pos[last];
		sf->vel[i]       = sf->vel[last];
		sf->scale[i]     = sf->scale[last];
		sf->mass[i]      = sf->mass[last];
		sf->rot[i]       = sf->rot[last];
		sf->rot_speed[i] = sf->rot_speed[last];
		sf->colour[i]    = sf->colour[last];
		sf->phase[i]     = sf->phase[last];
		sf->speed[i]     = sf->speed[last];
	}

	sf->count--;
}

static void stars_resolve_collisions( struct star_field *sf )
{
	int i, j;
	float radius_sum;
	float dist_sq;

	if ( !sf )
		return;

	for ( i = 0; i < sf->count; i++ ) {
		if ( sf->scale[i] < STAR_MERGE_MIN_SCALE )
			continue;

		for ( j = i + 1; j < sf->count; j++ ) {
			if ( sf->scale[j] < STAR_MERGE_MIN_SCALE )
				continue;

			radius_sum = ( sf->scale[i] + sf->scale[j] ) * STAR_RADIUS_FACTOR;
			dist_sq = vec2_dist_sq( sf->pos[i], sf->pos[j] );

			if ( dist_sq > radius_sum * radius_sum )
				continue;

			stars_merge( sf, i, j );
			stars_swap_remove( sf, j );

			j--;
		}
	}
}

static void stars_destroy( struct star_field *sf )
{
	if ( !sf )
		return;

	free( sf->model );
	free( sf->colour );
	free( sf->pos );
	free( sf->vel );
	free( sf->mass );
	free( sf->scale );
	free( sf->rot );
	free( sf->rot_speed );
	free( sf->phase );
	free( sf->speed );

	memset( sf, 0, sizeof( *sf ) );
}
