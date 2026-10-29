#ifndef ENGINE_AUDIO_H
#define ENGINE_AUDIO_H

#include <stdbool.h>

#include "miniaudio.h"

/**
 * struct audio_system - Audio output and mixer.
 * @engine: miniaudio engine. Must not be copied or moved after init.
 * @init: True once audio_system_init() has succeeded.
 *
 * Destroy every &struct sound created from it before calling audio_system_destroy().
 */
struct audio_system {
	ma_engine engine;
	bool init;
};

/**
 * audio_system_init() - Open the audio device and start the engine.
 * @audio: Zeroed audio system. Must not already be initialised.
 *
 * Return: true on success, false on failure or if @audio is NULL.
 */
bool audio_system_init( struct audio_system *audio );

/**
 * audio_play_once() - Play a file once, with no handle.
 * @audio: Initialised audio system.
 * @path: File path.
 *
 * Fire and forget: The engine manages the sound’s lifetime.
 * No explicit sound cleanup is required. It can't be stopped or adjusted afterwards.
 * The file is opened on each call.
 *
 * Return: true if playback started, false on bad arguments or failure.
 */
bool audio_play_once( struct audio_system *audio, const char *src );

void audio_system_destroy( struct audio_system *audio );

#endif
