// voice.h - // STT (upload PCM a STT cloud) + TTS (download MP3 da TTS cloud/proxy edge-tts).
#pragma once
#include <stdint.h>
#include <stdbool.h>

void voice_init(void);
void voice_speak(const char *text);   // TTS cloud -> MP3 -> audio out
int  voice_stt(const int16_t *pcm, int samples, char *out, int out_len);
