#include <stdio.h>
#include <stdlib.h>

#include "app/app.h"
#include "window/window.h"
#include "input/input.h"

#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"

#include "primitives/shape2d.h"
#include "primitives/colour.h"

#include "log.h"

#define WINDOW_TITLE         "ENGINE"
#define WINDOW_WIDTH         800
#define WINDOW_HEIGHT        600

#define SPEED                200.0f

#define MAX_BULLETS          3

static void create_bullet( struct shape2d *arr, int *count, vec2 pos, float tri_height )
{
	bool ok;

	if ( (*count) >= MAX_BULLETS )
		return;

	ok = shape2d_create( &arr[*count], SHAPE2D_RECTANGLE, pos.x,
	                     pos.y - tri_height * 0.5f, 5.0f, 30.0f );

	if ( !ok )
		return;

	shape2d_set_colour( &arr[*count], BLUE );
	(*count)++;
}

static void draw_bullet( struct shape2d *arr, int count, struct renderer_2d *r )
{
	int i;

	for ( i = 0; i < count; i++ )
		renderer_2d_draw_shape( r, &arr[i] );
}

static void update_bullet( struct shape2d *arr, int *b_count, float speed )
{
	int i;
	struct shape2d *bullet;

	for ( i = 0; i < *b_count; i++ ) {
		bullet = &arr[i];

		if ( !bullet )
			continue;

		shape2d_move( bullet, 0.0f, -speed );

		if ( bullet->pos.y + bullet->scale.y <= 0 ) {
			shape2d_destroy( bullet );
			arr[i] = arr[*b_count - 1];
			(*b_count)--;
			i--;
		}
		printf("BULLET: %p Y-POS (%.2f)\n", bullet , bullet->pos.y);
	}
}

int main( int argc, char *argv[] )
{
	struct app app;
	struct shape2d tri;
	struct renderer_2d renderer;

	struct shape2d bullets[MAX_BULLETS];
	int bullet_count;

	float dt, velocity;

	/*
	 * APP SETUP
	 * window, input, path, timer
	 */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return EXIT_FAILURE;

	/* SHAPE SETUP */
	if ( !shape2d_create( &tri, SHAPE2D_TRIANGLE, 400.0f, 100.0f, 40.0f, 40.0f ) )
		return EXIT_FAILURE;

	shape2d_set_colour( &tri, BLUE );

	/* RENDERER SETUP */
	if ( renderer_2d_init( &renderer, app.win.width, app.win.height ) != 0 )
		return EXIT_FAILURE;

	bullet_count = 0;

	while ( app.running ) {
		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

		if ( key_pressed( &app.input, KEY_ESCAPE ) )
			app_stop( &app );

		dt = (float)app.time.delta_time;
		velocity = SPEED * dt;

		/* POSITIONING-CONTROLS */
		if ( key_down( &app.input, KEY_A ) ) {
			shape2d_move( &tri, -velocity, 0.0f );
			printf("PLR_POS: (%.2f, %.2f)\n", tri.pos.x, tri.pos.y);
		}

		if ( key_down( &app.input, KEY_D ) ) {
			shape2d_move( &tri, velocity, 0.0f );
			printf("PLR_POS: (%.2f, %.2f)\n", tri.pos.x, tri.pos.y);
		}

		if ( key_down( &app.input, KEY_S ) ) {
			shape2d_move( &tri, 0.0f, velocity );
			printf("PLR_POS: (%.2f, %.2f)\n", tri.pos.x, tri.pos.y);
		}

		if ( key_down( &app.input, KEY_W ) ) {
			shape2d_move( &tri, 0.0f, -velocity );
			printf("PLR_POS: (%.2f, %.2f)\n", tri.pos.x, tri.pos.y);
		}

		/* ROTATIONAL-CONTROLS */
		if ( key_down( &app.input, KEY_LEFT ) ) {
			shape2d_rotate( &tri, -5.0f * dt );
			printf("PLR_ROT: %.1f\n", tri.rotation);
		}

		if ( key_down( &app.input, KEY_RIGHT ) ) {
			shape2d_rotate( &tri, 5.0f * dt );
			printf("PLR_ROT: %.1f\n", tri.rotation);
		}

		/* BULLET */
		if ( key_pressed( &app.input, KEY_ENTER ) )
			create_bullet( bullets, &bullet_count, tri.pos, tri.scale.y );

		update_bullet( bullets, &bullet_count, 700.0f * dt );
		draw_bullet( bullets, bullet_count, &renderer );

		renderer_2d_draw_shape( &renderer, &tri );

		app_update( &app );
	}

	shape2d_destroy( &tri );
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return EXIT_SUCCESS;
}
