#ifndef ENGINE_SOUND_H
#define ENGINE_SOUND_H

#include <stdbool.h>

#include "miniaudio.h"

struct audio_system;

enum sound_load_mode {
	SOUND_LOAD_DECODE,
	SOUND_LOAD_STREAM
};

/**
 * struct sound - One audio playback instance.
 * @handle: Backend sound.
 * @init
 */
struct sound {
	ma_sound handle;
	bool init;
};

bool sound_init( struct sound *sound, struct audio_system *audio,
                 const char *path, enum sound_load_mode mode );

void sound_destroy( struct sound *sound );

bool sound_play( struct sound *sound );
bool sound_pause( struct sound *sound );
bool sound_stop( struct sound *sound );
bool sound_restart( struct sound *sound );
void sound_set_volume( struct sound *sound, float volume );
void sound_set_loop( struct sound *sound, bool loop );

#endif
