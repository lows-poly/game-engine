#ifndef ENGINE_SOUND_H
#define ENGINE_SOUND_H

#include <stdbool.h>

#include "miniaudio.h"

struct audio_system;

/**
 * enum sound_load_flags - How a sound is loaded. Pick one source flag.
 * @SOUND_LOAD_DECODE: Decode fully into memory.
 * @SOUND_LOAD_STREAM: Decode at runtime.
 * @SOUND_LOAD_ASYNC: Load in the background. Combine with a source flag.
 *
 * With ASYNC, loads in the background after initial decoder setup.
 * Initialisation may still block briefly. Check sound_get_status() for completion.
 * */
enum sound_load_flags {
	SOUND_LOAD_DECODE = 0x1,
	SOUND_LOAD_STREAM = 0x2,
	SOUND_LOAD_ASYNC  = 0x4
};

/**
 * enum sound_status
 * @SOUND_STATUS_INVALID: NULL or uninitialised sound.
 * @SOUND_STATUS_LOADING: Background load in progress.
 * @SOUND_STATUS_READY: Loaded and playable.
 * @SOUND_STATUS_FAILED: Load failed. Destroy the sound.
 */
enum sound_status {
	SOUND_STATUS_INVALID,
	SOUND_STATUS_LOADING,
	SOUND_STATUS_READY,
	SOUND_STATUS_FAILED
};

/**
 * struct sound - One audio playback instance.
 * @handle: miniaudio sound. Must not be copied or moved after init.
 * @init: True once sound_init() has succeeded.
 *
 * Destroy before the owning &struct audio_system.
 */
struct sound {
	ma_sound handle;
	bool init;
};

bool sound_init( struct sound *sound, struct audio_system *audio,
                 const char *path, unsigned int flags );
enum sound_status sound_get_status( const struct sound *sound );
void sound_destroy( struct sound *sound );

bool sound_play( struct sound *sound );
bool sound_pause( struct sound *sound );
bool sound_stop( struct sound *sound );
bool sound_restart( struct sound *sound );
void sound_set_volume( struct sound *sound, float volume );
void sound_set_loop( struct sound *sound, bool loop );

bool sound_is_playing( const struct sound *sound );
bool sound_at_end( const struct sound *sound );
bool sound_is_looping( const struct sound *sound );

#endif
