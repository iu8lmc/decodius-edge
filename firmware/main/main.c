// main.c — entry point Decodius Edge (ESP32-P4).
#include "nvs_flash.h"
#include "esp_log.h"
#include "ui_lvgl.h"
#include "net.h"
#include "store.h"
#include "app_decodius.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "=== Decodius Edge ===");

    // NVS (config, chiavi cifrate, memoria persistente)
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    store_init();        // carica config (host Decodium, chiavi, nominativo)
    ui_init();           // display + LVGL: mostra l'orb (hello orb)
    net_start();         // Wi-Fi via ESP32-C6 + NTP (poi sblocca le funzioni cloud)

    app_decodius_start();   // orchestrazione
}
