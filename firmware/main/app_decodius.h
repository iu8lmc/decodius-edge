// app_decodius.h — macchina a stati dell'assistente (cuore dell'orchestrazione).
#pragma once

typedef enum {
    DEC_IDLE = 0,     // a riposo, in ascolto della wake-word
    DEC_LISTEN,       // sta registrando la voce dell'utente
    DEC_THINK,        // STT + LLM (+ tool) in corso
    DEC_SPEAK,        // sta riproducendo la risposta (TTS)
} dec_state_t;

// Avvia l'app: crea i task, registra le callback dei moduli.
void app_decodius_start(void);

// Imposta lo stato (aggiorna anche l'UI/orb).
void app_decodius_set_state(dec_state_t s);
dec_state_t app_decodius_state(void);

// Punto di ingresso di un turno: testo utente -> LLM/agent -> risposta -> TTS.
// (Chiamato dalla wake-word+STT o da input touch.)
void app_decodius_handle_text(const char *user_text);
