#include <stddef.h>

#include "mathx.h"
#include "audio.h"
#include "sound.h"

#define SOUND_LOAD_KNOWN ( SOUND_LOAD_DECODE | SOUND_LOAD_STREAM | SOUND_LOAD_ASYNC )

void sound_destroy( struct sound *sound )
{
	if ( !sound || !sound->init )
		return;

	ma_sound_uninit( &sound->handle );
	sound->init = false;
}

bool sound_init( struct sound *sound, struct audio_system *audio,
                 const char *path, unsigned int flags )
{
	ma_uint32 ma_flags = MA_SOUND_FLAG_NO_SPATIALIZATION;
	unsigned int source;

	if ( !sound )
		return false;

	sound->init = false;

	if ( !audio || !audio->init || !path || !path[0] )
		return false;

	if ( ( flags & ~(unsigned int)SOUND_LOAD_KNOWN ) != 0U )
		return false;

	source = flags & ( SOUND_LOAD_DECODE | SOUND_LOAD_STREAM );
	if ( source == SOUND_LOAD_DECODE )
		ma_flags |= MA_SOUND_FLAG_DECODE;
	else if ( source == SOUND_LOAD_STREAM )
		ma_flags |= MA_SOUND_FLAG_STREAM;
	else
		return false;

	if ( ( flags & SOUND_LOAD_ASYNC ) != 0U )
		ma_flags |= MA_SOUND_FLAG_ASYNC;

	if ( ma_sound_init_from_file( &audio->engine, path, ma_flags, NULL,
	                              NULL, &sound->handle ) != MA_SUCCESS )
	{
		return false;
	}

	sound->init = true;

	return true;
}

enum sound_status sound_get_status( const struct sound *sound )
{
	ma_data_source *ds;
	ma_result res;

	if ( !sound || !sound->init )
		return SOUND_STATUS_INVALID;

	ds = ma_sound_get_data_source( &sound->handle );
	if ( !ds )
		return SOUND_STATUS_INVALID;

	res = ma_resource_manager_data_source_result(
		(const ma_resource_manager_data_source *)ds );

	if ( res == MA_SUCCESS )
		return SOUND_STATUS_READY;
	if ( res == MA_BUSY )
		return SOUND_STATUS_LOADING;

	return SOUND_STATUS_FAILED;
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

	ma_sound_set_volume( &sound->handle, clampf( volume, 0.0f, 1.0f ) );
}
void sound_set_loop( struct sound *sound, bool loop )
{
	if ( !sound || !sound->init )
		return;

	ma_sound_set_looping( &sound->handle, loop ? MA_TRUE : MA_FALSE );
}

bool sound_is_playing( const struct sound *sound )
{
	if ( !sound || !sound->init )
		return false;

	return ma_sound_is_playing( &sound->handle ) == MA_TRUE &&
	       ma_sound_at_end( &sound->handle ) == MA_FALSE;
}

bool sound_at_end( const struct sound *sound )
{
	if ( !sound || !sound->init )
		return false;

	return ma_sound_at_end( &sound->handle ) == MA_TRUE;
}

bool sound_is_looping( const struct sound *sound )
{
	if ( !sound || !sound->init )
		return false;

	return ma_sound_is_looping( &sound->handle ) == MA_TRUE;
}
