// brain.h - // Client LLM + loop di tool-calling (decodium/hamqth/cluster/store) via cJSON+HTTP.
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef void (*brain_token_cb)(const char *chunk);
typedef void (*brain_done_cb)(const char *full_text);
void brain_init(void);
// Agent LLM (NVIDIA NIM, formato OpenAI): gestisce il tool-calling internamente.
void brain_ask(const char *user_text, brain_token_cb on_token, brain_done_cb on_done);
