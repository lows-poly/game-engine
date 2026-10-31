#include "app/app.h"
#include "renderer/renderer.h"
#include "renderer/renderer_2d.h"
#include "renderer/draw.h"

#include "graphics/sprite.h"
#include "graphics/sprite_animation.h"

#include "audio/sound.h"

#define WINDOW_TITLE	"GAME ENGINE"
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

#define WINDOW_CENTRE_X	( (float)WINDOW_WIDTH * 0.5f )
#define WINDOW_CENTRE_Y	( (float)WINDOW_HEIGHT * 0.5f )

#define SHAPE_WIDTH	48.0f
#define SHAPE_HEIGHT	48.0f

#define CANDLE_FRAME_WIDTH	32
#define CANDLE_FRAME_HEIGHT	32
#define CANDLE_FRAME_COUNT	4

#define CANDLE_SCALE	64.0f

#define TEXT_VISIBLE_TIME	1.0
#define TEXT_HIDDEN_TIME	0.5
#define TEXT_FLASH_CYCLE_TIME	( TEXT_VISIBLE_TIME + TEXT_HIDDEN_TIME )

int main( int argc, char *argv[] )
{
	struct app app;
	struct renderer_2d renderer;
	struct view_2d view;

	struct shape2d rect;
	struct shape2d tri;
	struct shape2d rect_tex;

	struct texture tex_template;
	struct texture candle;
	struct texture_config tex_config = TEXTURE_CONFIG_DEFAULT;
	struct texture_region frames[CANDLE_FRAME_COUNT];
	struct sprite sprite;
	struct sprite_animator animator;
	struct sprite_animation animation;

	struct font font_proggy;

	struct sound ambience;

	double flash_elapsed;
	int exit_status;
	int i;

	bool tex_ok;
	
	(void)argc;
	exit_status = -1;

	/* APP SETUP */
	if ( !app_init( &app, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, argv[0] ) ) {
		app_shutdown( &app );
		return EXIT_FAILURE;
	}

	/* RENDERER SETUP */
	if ( !renderer_2d_init( &renderer, WINDOW_WIDTH, WINDOW_HEIGHT ) )
		goto SHUTDOWN_EXIT;

	/* CENTRE VIEW */
	view = renderer.view;
	view.scale = VIEW_2D_FIT;
	view.anchor = VIEW_2D_CENTER;
	view.camera.pos = vec2_make( WINDOW_CENTRE_X, WINDOW_CENTRE_Y );

	if ( !renderer_2d_set_view( &renderer, &view ) )
		goto SHUTDOWN_EXIT;

	/* SHAPE SETUP */
	if ( !shape2d_create( &rect, SHAPE2D_RECTANGLE, SHAPE_WIDTH + 10.0f,
	                      WINDOW_CENTRE_Y, SHAPE_WIDTH, SHAPE_HEIGHT ) )
	{
		goto SHUTDOWN_EXIT;
	}

	if ( !shape2d_create( &tri, SHAPE2D_TRIANGLE, SHAPE_WIDTH + 80.0f,
	                      WINDOW_CENTRE_Y, SHAPE_WIDTH, SHAPE_HEIGHT ) )
	{
		goto DESTROY_SHAPES;
	}

	if ( !shape2d_create( &rect_tex, SHAPE2D_RECTANGLE, SHAPE_WIDTH + 160.0f,
	                      WINDOW_CENTRE_Y, SHAPE_WIDTH, SHAPE_HEIGHT ) )
	{
		goto DESTROY_SHAPES;
	}

	shape2d_set_colour( &tri, RED );

	/* TEXTURE SETUP */
	(void)texture_init( &tex_template );
	(void)texture_init( &candle );

	tex_config.min_filter = TEXTURE_NEAREST;
	tex_config.mag_filter = TEXTURE_NEAREST;

	tex_ok = texture_load( &tex_template, "assets/textures/dummy2.png", 0, NULL );
	tex_ok = texture_load( &candle, "assets/sprites/candle_lowres.png", 0, &tex_config );

	if ( !tex_ok )
		goto DESTROY_TEXTURES;

	/* SPRITE SETUP */
	for ( i = 0; i < CANDLE_FRAME_COUNT; i++ ) {
		frames[i] = (struct texture_region) {
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

	if ( !sprite_init( &sprite, &candle, &frames[0] ) )
		goto DESTROY_TEXTURES;

	if ( !sprite_animator_init( &animator, &sprite, &animation ) )
		goto DESTROY_TEXTURES;

	shape2d_set_scale( &sprite.shape, CANDLE_SCALE, CANDLE_SCALE );
	shape2d_center_origin( &sprite.shape );
	shape2d_set_pos( &sprite.shape, SHAPE_WIDTH + 10.0f, WINDOW_CENTRE_Y + 70.0f );

	/* FONT SETUP */
	if ( !font_init( &font_proggy, "assets/fonts/ProggyClean.ttf", 32.0f, NULL ) )
		goto DESTROY_FONT;

	/* DRAW SETUP */
	draw_set_renderer( &renderer );
	draw_set_font( &app.default_font );

	/* SOUND SETUP */
	if ( !sound_init( &ambience, &app.audio, "assets/audio/dreamcore_ambience.ogg",
	                  SOUND_LOAD_STREAM ) )
	{
		goto DESTROY_SOUND;
	}

	sound_set_volume( &ambience, 0.5f );
	sound_set_loop( &ambience, true );
	(void)sound_play( &ambience );

	/* THIS AUDIO PLAY ONCE */
	/* (void)audio_play_once( &app.audio, "assets/audio/annc_mtf.ogg" ); */

	flash_elapsed = 0.0;

	while ( app_begin_frame( &app ) ) {
		if ( key_pressed( &app.input, KEY_ESCAPE ) ) {
			app_stop( &app );
			break;
		}

		while ( timer_step( &app.timer ) ) {
			(void)sprite_animator_update( &animator, &sprite, app.timer.fixed_dt );

			flash_elapsed += app.timer.fixed_dt;

			while ( flash_elapsed >= TEXT_FLASH_CYCLE_TIME )
				flash_elapsed -= TEXT_FLASH_CYCLE_TIME;
		}

		renderer_begin_frame( BLACK );
		renderer_2d_update( &renderer, &app.win );

		draw_shape( &rect );
		draw_shape( &tri );

		draw_texture( &rect_tex, &tex_template, WHITE );
		draw_sprite( &sprite );

		draw_text( "Default font text rendering test", 30.0f, 20.0f, 1.0f, WHITE );

		renderer_2d_draw_text( &renderer, &font_proggy,
		                       "Proggy font text rendering test",
		                       30.0f, 43.0f, 0.45f, WHITE );

		if ( flash_elapsed < TEXT_HIDDEN_TIME )
			goto END_FRAME;

		draw_text( "PRESS [ESC] TO EXIT THE PROGRAM", 30.0f, 80.0f, 1.5f, RED );

	END_FRAME:
		app_end_frame( &app );
	}

	exit_status = 0;

DESTROY_SOUND:
	sound_destroy( &ambience );

DESTROY_FONT:
	font_destroy( &font_proggy );

DESTROY_TEXTURES:
	texture_destroy( &tex_template );
	texture_destroy( &candle );

DESTROY_SHAPES:
	shape2d_destroy( &rect );
	shape2d_destroy( &tri );
	shape2d_destroy( &rect_tex );
	// shape2d_destroy( &tri_tex );

SHUTDOWN_EXIT:
	renderer_2d_destroy( &renderer );
	app_shutdown( &app );

	return exit_status;
}
