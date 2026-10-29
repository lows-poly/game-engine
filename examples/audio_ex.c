#include <stdio.h>

#include "audio/audio.h"
#include "audio/sound.h"

int main( void )
{
	struct audio_system audio;
	struct sound track;
	int status = -1;

	if ( !audio_system_init( &audio ) ) {
		fprintf( stderr, "FAILED TO INITIALISE AUDIO SYSTEM\n" );
		goto CLEAN_AUDIO;
	}

	if ( !sound_init( &track, &audio, "assets/audio/annc_mtf.ogg", SOUND_LOAD_STREAM ) ) {
		fprintf( stderr, "FAILED TO LOAD SOUND\n" );
		goto CLEAN_SOUND;
	}

	sound_set_volume( &track, 1.0f );
	sound_set_loop( &track, true );

	if ( !sound_play( &track ) ) {
		fprintf( stderr, "FAILED TO START PLAYBACK\n" );
		goto CLEAN_SOUND;
	}

	printf("PRESS [ENTER] TO QUIT: ");
	(void)getchar();

	status = 0;

CLEAN_SOUND:
	sound_destroy( &track );
CLEAN_AUDIO:
	audio_system_destroy( &audio );

	return status;
}
