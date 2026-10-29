#include <stddef.h>
#include <math.h>

#include "audio.h"
#include "sound.h"

void sound_destroy( struct sound *sound )
{
	if ( !sound || !sound->init )
		return;

	ma_sound_uninit( &sound->handle );
	sound->init = false;
}

bool sound_init( struct sound *sound, struct audio_system *audio,
                 const char *path, enum sound_load_mode mode )
{
	ma_uint32 flags = MA_SOUND_FLAG_NO_SPATIALIZATION;

	if ( !sound )
		return false;

	sound->init = false;

	if ( !audio || !audio->init || !path || !path[0] )
		return false;

	switch ( mode ) {
	case SOUND_LOAD_DECODE:
		flags |= MA_SOUND_FLAG_DECODE;
		break;
	case SOUND_LOAD_STREAM:
		flags |= MA_SOUND_FLAG_STREAM;
		break;
	default:
		return false;
	}

	if ( ma_sound_init_from_file( &audio->engine, path, flags, NULL, NULL,
	                              &sound->handle ) != MA_SUCCESS )
	{
		return false;
	}

	sound->init = true;

	return true;
}

bool sound_play( struct sound *sound )
{
	if ( !sound || !sound->init )
		return false;

	return ma_sound_start( &sound->handle ) == MA_SUCCESS;
}
bool sound_pause( struct sound *sound )
{
	if ( !sound || !sound->init )
		return false;

	return ma_sound_stop( &sound->handle ) == MA_SUCCESS;
}

bool sound_stop( struct sound *sound )
{
	if ( !sound || !sound->init )
		return false;

	if ( ma_sound_stop( &sound->handle ) != MA_SUCCESS )
		return false;

	return ma_sound_seek_to_pcm_frame( &sound->handle, 0 ) == MA_SUCCESS;
}

bool sound_restart( struct sound *sound )
{
	if ( !sound_stop( sound ) )
		return false;

	return sound_play( sound );
}

void sound_set_volume( struct sound *sound, float volume )
{
	if ( !sound || !sound->init )
		return;

	if ( !isfinite( volume ) || volume < 0.0f || volume > 1.0f )
		return;

	ma_sound_set_volume( &sound->handle, volume );
}
void sound_set_loop( struct sound *sound, bool loop )
{
	if ( !sound || !sound->init )
		return;

	ma_sound_set_looping( &sound->handle, loop ? MA_TRUE : MA_FALSE );
}
