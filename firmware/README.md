# Decodius Edge — Firmware (ESP-IDF / ESP32-P4)

Scaffold del firmware. Allo stato attuale fornisce la **struttura del progetto** e un
**"hello orb"** LVGL; la logica dei moduli è da implementare seguendo
[`../docs/ARCHITETTURA.md`](../docs/ARCHITETTURA.md).

## Prerequisiti
- **ESP-IDF ≥ 5.3** ([guida](https://docs.espressif.com/projects/esp-idf/))
- Target **esp32p4**
- Display + audio + ESP32-C6 (vedi [`../docs/HARDWARE.md`](../docs/HARDWARE.md))

## Build & flash
```bash
. $IDF_PATH/export.sh
idf.py set-target esp32p4
idf.py build
idf.py -p /dev/ttyACM0 flash monitor
```

> Il Component Manager scarica `lvgl` ed `esp_lvgl_port` da `main/idf_component.yml`.
> Le altre dipendenze (esp-sr, esp_codec_dev, esp_wifi_remote) si abilitano man mano.

## Struttura
```
firmware/
├── CMakeLists.txt · sdkconfig.defaults · partitions.csv
├── main/            main.c · app_decodius.{c,h} · idf_component.yml
└── components/
    ├── ui_lvgl/     UI + orb (hello orb implementato)
    ├── net/         Wi-Fi (C6) + NTP + TLS
    ├── brain/       agent LLM (tool-calling)
    ├── decodium/    client API Decodium 4
    ├── voice/       STT + TTS cloud
    ├── audio/       I2S + codec + MP3
    ├── wakeword/    esp-sr WakeNet "Decodius"
    ├── hamqth/      lookup nominativi
    ├── cluster/     spot DX (dxwatch)
    └── store/       NVS/SD: config, memoria
```

## Prossimi passi (roadmap firmware)
1. **Display**: completare `ui.c` con il driver del pannello (MIPI-DSI/RGB) + touch GT911 via `esp_lvgl_port`.
2. **net**: Wi-Fi via ESP32-C6 (esp_wifi_remote) + NTP.
3. **voice/audio**: TTS cloud → MP3 → I2S (Decodius "parla").
4. **brain**: client LLM (NVIDIA NIM) + loop tool-calling (cJSON).
5. **decodium/hamqth/cluster**: i tool HTTP.
6. **wakeword**: WakeNet "Decodius" + STT cloud → `app_decodius_handle_text()`.
7. **store**: NVS cifrata (chiavi) + memoria persistente.

Vedi la sezione *Roadmap a fasi* in `../docs/ARCHITETTURA.md`.
