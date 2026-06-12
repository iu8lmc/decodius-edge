// wakeword.h - // Wake-word on-device (esp-sr WakeNet) con AEC/NS/VAD dell'AFE.
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef void (*wakeword_cb)(void);
void wakeword_start(wakeword_cb on_wake);  // WakeNet 'Decodius' + AFE
