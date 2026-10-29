#include <stddef.h>

#include "audio.h"

bool audio_system_init( struct audio_system *audio )
{
	if ( !audio )
		return false;

	audio->init = false;

	if ( ma_engine_init( NULL, &audio->engine ) != MA_SUCCESS )
		return false;

	audio->init = true;

	return true;
}

void audio_system_destroy( struct audio_system *audio )
{
	if ( !audio || !audio->init )
		return;

	ma_engine_uninit( &audio->engine );
	audio->init = false;
}
