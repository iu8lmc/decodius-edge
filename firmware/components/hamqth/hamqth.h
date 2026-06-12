// hamqth.h - // Lookup nominativi HamQTH (XML).
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef void (*hamqth_cb)(const char *json_card);
// Login (sessione riusata) + lookup -> callback con i campi (nome,qth,paese,grid,lat,lon).
void hamqth_lookup(const char *call, hamqth_cb cb);
