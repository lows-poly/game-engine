#include "app/app.h"
#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"

#include "graphics/sprite.h"
#include "graphics/sprite_animation.h"

#define WINDOW_TITLE	"SPRITE ANIMATION"
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

#define RECT_WIDTH	256.0f
#define RECT_HEIGHT	256.0f

#define CANDLE_FRAME_WIDTH	32
#define CANDLE_FRAME_HEIGHT	32
#define CANDLE_FRAME_COUNT	4

#define CANDLE_SCALE	128.0f

int main( int argc, char *argv[] )
{
	struct app app;
	struct renderer_2d renderer;
	struct view_2d view;
	
	struct texture texture;
	struct texture_region frames[CANDLE_FRAME_COUNT];

	struct sprite sprite;
	struct sprite_animator animator;
	struct sprite_animation animation;

	int i;

	(void)argc;

	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) )
		return -1;

	if ( !renderer_2d_init( &renderer, WINDOW_WIDTH, WINDOW_HEIGHT ) )
		goto SHUTDOWN;

	view = renderer.view;
	view.scale = VIEW_2D_FIT;
	view.anchor = VIEW_2D_CENTER;
	view.camera.pos = vec2_make( (float)WINDOW_WIDTH * 0.5f, (float)WINDOW_HEIGHT * 0.5f );

	if ( !renderer_2d_set_view( &renderer, &view ) )
		goto CLEAN_RENDERER;

	if ( !texture_init( &texture ) )
		goto CLEAN_RENDERER;

	if ( !texture_load( &texture, "assets/sprites/candle_lowres.png", 0, NULL ) )
		goto CLEAN_TEXTURE;

	if ( !texture_set_filter( &texture, TEXTURE_NEAREST, TEXTURE_NEAREST ) )
		goto CLEAN_TEXTURE;

	for ( i = 0; i < CANDLE_FRAME_COUNT; i++ ) {
		frames[i] = (struct texture_region){
			.x = i * CANDLE_FRAME_WIDTH,
			.y = 0,
			.width = CANDLE_FRAME_WIDTH,
			.height = CANDLE_FRAME_HEIGHT
		};
	}

	animation = (struct sprite_animation){
		.frames = frames,
		.frame_count = CANDLE_FRAME_COUNT,
		.frame_dt = 0.25,
		.loop = true
	};

	if ( !sprite_init( &sprite, &texture, &frames[0] ) )
		goto CLEAN_TEXTURE;

	if ( !sprite_animator_init( &animator, &sprite, &animation ) )
		goto CLEAN_TEXTURE;

	shape2d_set_scale( &sprite.shape, CANDLE_SCALE, CANDLE_SCALE );
	shape2d_center_origin( &sprite.shape );
	shape2d_set_pos( &sprite.shape, (float)WINDOW_WIDTH * 0.5f, (float)WINDOW_HEIGHT * 0.5f );

	while ( app_begin_frame( &app ) ) {
		if ( key_pressed( &app.input, KEY_ESCAPE ) ) {
			app_stop( &app );
			break;
		}

		if ( key_pressed( &app.input, KEY_SPACE ) )
			animator.playing = !animator.playing;

		if ( key_pressed( &app.input, KEY_COMMA ) )
			sprite_animator_reset( &animator, &sprite );

		if ( key_pressed( &app.input, KEY_PERIOD ) )
			animation.loop = !animation.loop;

		while ( timer_step( &app.timer ) ) {
			if ( !sprite_animator_update( &animator, &sprite, app.timer.fixed_dt ) )
				goto CLEAN_TEXTURE;
		}

		renderer_2d_update( &renderer, &app.win );
		renderer_begin_frame( BLACK );
		renderer_2d_draw_sprite( &renderer, &sprite );

		app_end_frame( &app );
	}

	texture_destroy( &texture );
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return 0;

CLEAN_TEXTURE:
	texture_destroy( &texture );

CLEAN_RENDERER:
	renderer_2d_destroy( &renderer );

SHUTDOWN:
	app_shutdown( &app );

	return -1;
}
