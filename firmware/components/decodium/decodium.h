// decodium.h - // Client API Decodium 4 (LAN): /api/state, /api/decodes, /api/v1/commands.
#pragma once
#include <stdint.h>
#include <stdbool.h>

void decodium_get_state(char *out, int out_len);
void decodium_get_decodes(char *out, int out_len);
// Comando: type es. set_band/set_mode/set_rx_frequency/select_caller/set_*_enabled.
int  decodium_command(const char *json_body);
