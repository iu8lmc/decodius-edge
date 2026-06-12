// audio.h - // I2S mic+speaker, codec ES8311, decode MP3 (libhelix).
#pragma once
#include <stdint.h>
#include <stdbool.h>

void audio_init(void);
void audio_play_mp3(const uint8_t *data, int len);
void audio_capture_start(void);   // I2S mic -> buffer
