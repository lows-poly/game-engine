#include <stddef.h>

#include "audio.h"

/**
 * audio_system_init() - Open the audio device and start the engine.
 * @audio: Zeroed audio system. Must not already be initialised.
 *
 * Return: true on success, false on failure or if @audio is NULL.
 */
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

/**
 * audio_play_once() - Play a file once, with no handle.
 * @audio: Initialised audio system.
 * @path: File path.
 *
 * Fire and forget: the sound cleans itself up when it finishes, and it
 * can't be stopped, paused, or adjusted afterwards. The file is opened on
 * each call.
 *
 * Return: true if playback started, false on bad arguments or failure.
 */
bool audio_play_once( struct audio_system *audio, const char *src )
{
	if ( !audio || !audio->init || !src || !src[0] )
		return false;

	return ma_engine_play_sound( &audio->engine, src, NULL ) == MA_SUCCESS;
}

void audio_system_destroy( struct audio_system *audio )
{
	if ( !audio || !audio->init )
		return;

	ma_engine_uninit( &audio->engine );
	audio->init = false;
}
