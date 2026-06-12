// store.h - // Config + chiavi (NVS cifrata) + memoria/log su FAT/SD.
#pragma once
#include <stdint.h>
#include <stdbool.h>

void store_init(void);
bool store_get(const char *key, char *out, int out_len);
bool store_set(const char *key, const char *val);
// Memoria persistente: ricorda fatti tra le sessioni.
void store_memory_add(const char *fact);
