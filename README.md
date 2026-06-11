# Decodius Edge — ESP32-P4

**Decodius Edge** è la versione *embedded* di [Decodius](https://github.com/iu8lmc/Decodius):
un dispositivo da tavolo dedicato (stile Echo Show) costruito attorno a un
**ESP32-P4**, che porta l'assistente vocale radioamatoriale e l'integrazione con
**Decodium 4** su un display touch autonomo — senza PC.

> ⚠️ Progetto firmware **nuovo** (ESP-IDF + LVGL), NON un port dell'app Qt/Python.
> Intelligenza (LLM), STT e TTS girano in **cloud**; il P4 fa UI, audio e orchestrazione.

Autore: **Martino — IU8LMC**

---

## Idea in una riga
> Un "thin client" intelligente: il P4 ascolta, mostra e parla; il cervello e la
> voce stanno nel cloud, Decodium sta sulla LAN.

```
🎙 voce → wake-word locale (P4) → STT cloud → LLM cloud (+ tool) → TTS cloud → 🔊
                                                   │
                                                   └── comandi/letture → Decodium 4 (LAN)
🖥 UI LVGL: orb · HUD · scheda QRZ · roster · mappa
```

## Documenti
- [`docs/ARCHITETTURA.md`](docs/ARCHITETTURA.md) — architettura completa, moduli, flussi, struttura firmware
- [`docs/HARDWARE.md`](docs/HARDWARE.md) — BOM, schema collegamenti, scelte componenti

## Perché ESP32-P4
- Dual-core **RISC-V HP @400 MHz** + LP core, cache, PSRAM esterna (fino a 32 MB)
- **MIPI-DSI / RGB / 8080** per display, **2D PPA** (accelerazione grafica), H.264
- **esp-sr** (WakeNet/MultiNet): wake-word "Decodius" **on-device**
- I2S audio, USB-HS, SDMMC
- Connettività via **ESP32-C6** affiancato (Wi-Fi 6 / BLE, esp-hosted)

## Stato
📐 Fase di **design** — questa cartella contiene la sola architettura, non ancora il firmware.
