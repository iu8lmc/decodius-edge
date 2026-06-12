// ui.c — bring-up display + LVGL + home con orb.
//
// NOTA: l'inizializzazione del pannello è specifica della scheda/display.
// Qui c'è lo scheletro con esp_lvgl_port; sostituire il blocco "TODO display" con
// il driver del tuo pannello (MIPI-DSI o RGB) e del touch (GT911).
#include "ui_lvgl.h"
#include "esp_log.h"
#include "lvgl.h"
// #include "esp_lvgl_port.h"   // abilitare quando la dipendenza è risolta

static const char *TAG = "ui";

void ui_init(void)
{
    ESP_LOGI(TAG, "UI init");

    // TODO display: inizializza il pannello (MIPI-DSI/RGB) + touch, poi:
    //   lvgl_port_init(...);
    //   lv_display_t *disp = lvgl_port_add_disp(...);
    // Per ora si assume che LVGL sia già avviato dal port.

    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x06121b), 0);

    // Wordmark
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "DECODIUS");
    lv_obj_set_style_text_color(title, lv_color_hex(0xeaf6fb), 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 16, 14);

    // Orb centrale
    orb_create(scr);

    // Stato testuale
    lv_obj_t *st = lv_label_create(scr);
    lv_label_set_text(st, "PRONTO");
    lv_obj_set_style_text_color(st, lv_color_hex(0x36b6e0), 0);
    lv_obj_align(st, LV_ALIGN_BOTTOM_MID, 0, -20);
}

void ui_set_state(int state)       { orb_set_state(state); }
void ui_set_audio_level(float rms) { orb_set_level(rms); }

// Stub dei pannelli/chat — da implementare con LVGL.
void ui_chat_user(const char *text)        { ESP_LOGI(TAG, "[tu] %s", text ? text : ""); }
void ui_chat_append(const char *chunk)     { /* TODO: append al label/list chat */ (void)chunk; }
void ui_hud_update(const char *l1, const char *l2, bool online) { (void)l1; (void)l2; (void)online; }
void ui_show_card(const char *json_card)   { (void)json_card; /* TODO: overlay scheda + mappa */ }
