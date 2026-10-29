#ifndef ENGINE_AUDIO_H
#define ENGINE_AUDIO_H

#include <stdbool.h>

#include "miniaudio.h"

/**
 * audio_system - Audio output and mixer.
 * @engine: miniaudio engine.
 * @init
 */

struct audio_system {
	ma_engine engine;
	bool init;
};

bool audio_system_init( struct audio_system *audio );

void audio_system_destroy( struct audio_system *audio );

#endif
