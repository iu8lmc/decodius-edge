# Decodius Edge — Hardware (BOM e collegamenti)

## 1. Componenti principali (BOM)

| # | Componente | Funzione | Note |
|---|-----------|----------|------|
| 1 | **ESP32-P4** (modulo o devkit, es. ESP32-P4-Function-EV-Board) | SoC principale: UI, audio, orchestrazione | RISC-V 400 MHz, MIPI-DSI, PPA |
| 2 | **PSRAM 16–32 MB** | Framebuffer + buffer LVGL/JSON | spesso integrata nel modulo |
| 3 | **Flash 16 MB** | App + modello wake-word + asset | |
| 4 | **ESP32-C6** (o modulo combo) | Wi-Fi 6 / BLE via esp-hosted | il P4 non ha radio integrata |
| 5 | **Display MIPI-DSI 7–10"** touch (es. 1024×600) | UI | alternativa: RGB/8080 |
| 6 | **Touch capacitivo** (GT911 I2C) | input | spesso integrato col pannello |
| 7 | **Codec audio I2S** (ES8311) + **PA** (NS4150) | speaker out | |
| 8 | **Microfono** I2S/PDM (es. MSM261) ×1–2 | voce in (1–2 mic per AEC) | |
| 9 | **Altoparlante** 4–8 Ω 2–3 W | uscita voce | |
| 10 | **Alimentazione** USB-C 5 V / ≥2 A | | display + audio assorbono |
| 11 | (opz.) **microSD** | KB, log QSO, asset extra | SDMMC |
| 12 | (opz.) encoder rotativo / pochi tasti | volume / navigazione | |

## 2. Schema collegamenti (logico)

```
ESP32-P4
 ├─ MIPI-DSI  ───────────── Display 7–10" (1024×600)
 ├─ I2C0  ───────────────── Touch GT911  +  Codec ES8311 (control)
 ├─ I2S0 (TX) ───────────── ES8311 → PA NS4150 → Altoparlante
 ├─ I2S1 (RX/PDM) ───────── Microfono MSM261 (×1–2)
 ├─ SDIO/SPI (esp-hosted) ─ ESP32-C6  (Wi-Fi/BLE)
 ├─ SDMMC ───────────────── microSD (opzionale)
 └─ USB-C ───────────────── alimentazione + flashing/log
```

## 3. Note di progetto
- **Audio full-duplex** per il barge-in: mic attivo durante il TTS → serve **AEC** (cancellazione d'eco) dell'AFE esp-sr, altrimenti l'assistente si "sente" e si interrompe da solo (lo stesso problema del desktop, lì risolto evitando il barge-in).
- **2 microfoni** migliorano AEC/beamforming (consigliati se si vuole barge-in stabile).
- **MIPI-DSI** è la via più pulita per i 7–10"; in alternativa pannelli **RGB** (più pin, meno banda).
- Il **PPA** del P4 accelera blit/scaling LVGL → UI fluida anche a 1024×600.
- Prevedere **dissipazione** minima (il P4 a pieno carico + display scaldano).

## 4. Form factor
Appliance da tavolo "Echo Show": pannello frontale touch inclinato, speaker frontale,
mic in alto, base con USB-C posteriore. Stampa 3D del case + supporto.

## 5. Costo indicativo (prototipo)
Devkit P4 + display MIPI 7" touch + scheda audio + C6 ≈ fascia **hobby** (decine di €),
ben sotto un mini-PC, con consumi e ingombro da "appliance".
