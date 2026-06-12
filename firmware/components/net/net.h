// net.h - // Wi-Fi via ESP32-C6 (esp-hosted) + NTP, poi sblocca le funzioni cloud.
#pragma once
#include <stdint.h>
#include <stdbool.h>

void net_start(void);
bool net_is_connected(void);
