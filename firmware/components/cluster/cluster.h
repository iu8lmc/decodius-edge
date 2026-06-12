// cluster.h - // Spot DX dal cluster mondiale (dxwatch).
#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef void (*cluster_cb)(const char *json_spots);
void cluster_spots(const char *band, cluster_cb cb);  // dxwatch JSON
