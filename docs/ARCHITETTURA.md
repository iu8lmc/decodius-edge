# Decodius Edge — Architettura (ESP32-P4)

## 1. Visione e principi

Decodius Edge è un **terminale dedicato**: tutta l'intelligenza pesante è in cloud o
sulla LAN, il microcontrollore fa ciò in cui è bravo — **UI fluida, audio I/O,
wake-word locale e orchestrazione HTTP**.

Principi:
- **Niente LLM/STT pesanti on-device** (impossibile su MCU): si usano API cloud.
- **Wake-word locale** ("Decodius") con esp-sr, per reattività e privacy a riposo.
- **Orchestrazione sul P4**: il firmware fa da agente (chiama LLM, esegue i tool, parla).
- **Decodium sulla LAN**: stesse API HTTP usate dall'app desktop (`:8080` lettura, `:19091` comandi).

---

## 2. Diagramma a blocchi

```
            ┌──────────────────────────── ESP32-P4 ────────────────────────────┐
            │                                                                   │
  🎤 MIC ──▶│  I2S in ─▶ [Wake-word esp-sr] ─▶ [Audio capture/VAD]              │
            │                     │"Decodius"           │ frame PCM             │
            │                     ▼                      ▼                       │
            │              [UI/State LVGL]        [STT client] ───HTTP TLS──────┼──▶ ☁ STT cloud
            │                  orb/hud/card             ▲ testo                 │     (Whisper API/
            │                  roster/map               │                       │      Deepgram)
            │                     ▲                      ▼                       │
  🖥 DISPLAY◀│  MIPI-DSI ◀─ [LVGL render + PPA]    [Brain/Agent] ──HTTP TLS──────┼──▶ ☁ LLM cloud
   (touch) ─▶│  touch I2C ─▶                          │ tool calls             │     (NVIDIA NIM)
            │                                          ├─HTTP──(LAN)────────────┼──▶ 📡 Decodium 4
            │                                          │  state/decodes/cmd     │     :8080 / :19091
            │                                          ├─HTTP TLS───────────────┼──▶ 🌍 HamQTH / dxwatch
            │                                          ▼                        │
            │              [TTS client] ◀── testo risposta                      │
            │                  │ MP3                                            │
            │                  ▼                                                │
  🔊 SPK ◀──│  I2S out ◀─ [MP3 decode] ◀─────────────────────────HTTP TLS──────┼──▶ ☁ TTS cloud
            │                                                                   │     (edge-tts proxy)
            │   [Store: NVS/SD]  memoria · config · KB · log QSO                │
            └─────────────────┬─────────────────────────────────────────────────┘
                              │ SDIO/SPI (esp-hosted)
                       ┌──────▼──────┐
                       │  ESP32-C6   │  Wi-Fi 6 / BLE
                       └─────────────┘
```

---

## 3. Flussi principali

### 3.1 Conversazione vocale (a mani libere)
1. **Wake-word** "Decodius" → WakeNet (esp-sr) sul P4, sempre in ascolto a basso consumo.
2. **Cattura + VAD**: registra finché parli (AFE di esp-sr per noise suppression/AEC).
3. **STT**: invia il PCM a un'API cloud → testo.
4. **Agent (Brain)**: invia testo + cronologia + **tool schema** al LLM (NVIDIA NIM, formato OpenAI). 
5. Se il LLM risponde con **tool_calls** → il firmware li esegue (Decodium/HamQTH/cluster/memoria) e rimanda i risultati (loop, come nell'app desktop).
6. **TTS**: la risposta finale va a un TTS cloud → MP3 → decode → I2S → altoparlante.
7. **UI**: l'orb reagisce allo stato (ascolto/pensa/parla), i pannelli si aggiornano.

### 3.2 Comando alla radio
`"vai in 40 metri"` → tool `decodium_comando` → POST `http://<decodium>:19091/api/v1/commands`
con `{type:set_band, band:"40m"}` (loopback/LAN). Conferma vocale.

### 3.3 Scheda QRZ
`"scheda di W1AW"` → HamQTH login+lookup → card LVGL con dati + **marker su planisfero**.

---

## 4. Moduli software (component ESP-IDF)

| Component | Ruolo | Librerie chiave |
|-----------|-------|-----------------|
| `ui_lvgl` | UI: orb, HUD Decodium, scheda QRZ, roster, mappa, temi | **LVGL 9**, esp_lvgl_port |
| `wakeword` | "Decodius" on-device + AFE (AEC/NS/VAD) | **esp-sr** (WakeNet/MultiNet) |
| `audio` | I2S mic+speaker, codec, decode MP3 | esp_codec_dev, **libhelix-mp3** |
| `net` | Wi-Fi via ESP32-C6 (esp-hosted), TLS, NTP | esp_wifi_remote, esp-tls, mbedTLS |
| `brain` | Agent LLM: client NVIDIA NIM, tool calling, SSE | esp_http_client, **cJSON** |
| `voice` | Client STT (upload PCM) + client TTS (download MP3) | esp_http_client |
| `decodium` | Lettura `/api/state` `/api/decodes` + comandi `/api/v1/commands` | esp_http_client, cJSON |
| `hamqth` | Login + lookup nominativi (XML) | esp_http_client |
| `cluster` | Spot DX (dxwatch JSON) | esp_http_client, cJSON |
| `store` | Memoria persistente, config, KB, log ADIF | **NVS**, FATFS/SD |

Tutti i tool del Decodius desktop sono replicabili in C (sono già "agenti HTTP").

---

## 5. UI (LVGL) — schermate

- **Home / Orb**: sfera pulsante reattiva all'audio (RMS/FFT da I2S), stato a colori, anello spettrale.
- **HUD Decodium**: modo · banda · frequenza · TX/decodifiche (poll `/api/state` 3 s).
- **Call Roster**: lista stazioni in banda (`/api/decodes`), badge CQ, SNR colorato; vista **Mappa** (planisfero + marker per locatore).
- **Scheda QRZ**: overlay con mappa + dati HamQTH.
- **Quick actions** + **temi colore** (come nel desktop).

LVGL gira a ~30–60 fps sul P4 grazie al **PPA** (blit/scaling) e al doppio framebuffer in PSRAM.

---

## 6. Sicurezza / config
- Chiavi (NVIDIA, HamQTH) e credenziali in **NVS cifrata** (flash encryption + secure boot v2).
- TLS su tutte le chiamate cloud; Decodium su LAN/loopback.
- Setup iniziale via **captive portal** (Wi-Fi provisioning) o app companion: SSID, chiavi, indirizzo Decodium, nominativo.

---

## 7. Struttura del firmware (proposta)

```
firmware/
├── CMakeLists.txt
├── sdkconfig.defaults          # PSRAM, MIPI-DSI, esp-sr, flash enc.
├── partitions.csv              # app + nvs + model (wakenet) + storage
├── main/
│   ├── main.c                  # init board, avvio task
│   └── app_decodius.c          # macchina a stati (idle/listen/think/speak)
└── components/
    ├── ui_lvgl/                # schermate e widget
    ├── wakeword/               # esp-sr WakeNet "Decodius" + AFE
    ├── audio/                  # i2s, codec, mp3 decode
    ├── net/                    # wifi(c6)/tls/ntp/provisioning
    ├── brain/                  # agent LLM + tool dispatch
    ├── voice/                  # stt/tts client
    ├── decodium/               # client API Decodium 4
    ├── hamqth/ cluster/        # lookup + spot DX
    └── store/                  # nvs/sd: memoria, config, kb, log
```

---

## 8. Budget risorse (stima)
- **PSRAM**: 2× framebuffer (es. 1024×600×2B ≈ 1.2 MB l'uno) + LVGL buffer + JSON/TLS → consigliati **16–32 MB** PSRAM.
- **Flash**: app + modello WakeNet (~500 KB–1 MB) + asset (planisfero, font) → **16 MB** flash.
- **Heap interno**: TLS (mbedTLS ~40–60 KB/connessione), audio, LVGL.
- **Rete**: chiamate cloud sequenziali; latenza tipica voce ~1–3 s end-to-end (STT+LLM+TTS).

---

## 9. Roadmap a fasi
1. **Bring-up**: board + display LVGL "hello orb" + Wi-Fi (C6) + NTP.
2. **Decodium**: HUD live (lettura `/api/state`,`/api/decodes`) + comandi.
3. **Voce out**: TTS cloud → MP3 → I2S (Decodius "parla").
4. **Voce in**: wake-word locale + STT cloud + agent LLM (tool calling).
5. **Pannelli**: roster, mappa, scheda QRZ, temi.
6. **Hardening**: NVS cifrata, secure boot, provisioning, OTA.

---

## 10. Limiti e alternative
- Dipende da **Internet** per cervello/voce (come il desktop). Offline: solo wake-word + UI + Decodium LAN.
- Se si vuole più potenza locale/semplicità, un **mini-PC (N100) + display touch** esegue Decodius desktop *as-is* (zero porting) — opzione più rapida; l'ESP32-P4 vince su **costo, consumi, forma-fattore "appliance"**.
