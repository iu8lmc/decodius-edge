// app_decodius.c — orchestrazione: stato + flusso voce/agente.
// Replica, in C, il ciclo dell'app desktop: testo -> LLM (con tool) -> risposta -> voce.
#include "app_decodius.h"
#include "ui_lvgl.h"
#include "brain.h"
#include "voice.h"
#include "esp_log.h"

static const char *TAG = "decodius";
static dec_state_t s_state = DEC_IDLE;

dec_state_t app_decodius_state(void) { return s_state; }

void app_decodius_set_state(dec_state_t s)
{
    s_state = s;
    ui_set_state(s);          // l'orb riflette lo stato (idle/listen/think/speak)
}

// Callback dal Brain: token di testo in streaming (mostralo nella chat).
static void on_brain_token(const char *chunk)
{
    ui_chat_append(chunk);
}

// Callback dal Brain: risposta completa -> pronuncia.
static void on_brain_done(const char *text)
{
    app_decodius_set_state(DEC_SPEAK);
    voice_speak(text);        // TTS cloud -> MP3 -> I2S
}

void app_decodius_handle_text(const char *user_text)
{
    if (!user_text || !user_text[0]) return;
    ESP_LOGI(TAG, "utente: %s", user_text);
    ui_chat_user(user_text);
    app_decodius_set_state(DEC_THINK);
    // L'agent gestisce i tool (decodium/hamqth/cluster/memoria) internamente.
    brain_ask(user_text, on_brain_token, on_brain_done);
}

void app_decodius_start(void)
{
    ESP_LOGI(TAG, "Decodius Edge avviato");
    brain_init();
    voice_init();
    app_decodius_set_state(DEC_IDLE);
    // TODO: registrare la callback della wake-word -> avvia STT -> app_decodius_handle_text()
}
