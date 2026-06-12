// ui_lvgl.h — interfaccia UI (LVGL) di Decodius Edge.
#pragma once

// Inizializza display + LVGL e mostra la home (orb pulsante).
void ui_init(void);

// Aggiorna lo stato visivo dell'orb (0=idle,1=listen,2=think,3=speak — vedi dec_state_t).
void ui_set_state(int state);

// Reattività audio dell'orb (RMS 0..1 dall'I2S del microfono).
void ui_set_audio_level(float rms);

// Chat
void ui_chat_user(const char *text);     // bolla utente
void ui_chat_append(const char *chunk);  // token in streaming (risposta)

// Pannelli (HUD Decodium, scheda QRZ, roster/mappa) — implementazione futura.
void ui_hud_update(const char *line1, const char *line2, bool online);
void ui_show_card(const char *json_card); // scheda nominativo (HamQTH)

// ── orb.c ──
void orb_create(void *parent);   // crea il widget orb sul parent LVGL
void orb_set_state(int state);
void orb_set_level(float rms);
