# BK-Shoot — Arduino + MIT App Inventor basketball shot tracker

A low-cost hoop-mounted gadget + Android app that detects **makes/misses** in real time and tracks shooting stats. Built with **Arduino** sensors (IR + vibration) and a **Bluetooth** link to a MIT App Inventor app.

> Status: **v1.0 (public release)** · License: **MIT** · 

---

## Table of contents
- [Demo](#demo)
- [Features](#features)
- [Hardware](#hardware)
- [How it works](#how-it-works)
- [Quickstart](#quickstart)
- [Mobile app](#mobile-app)
- [Testing & validation](#testing--validation)
- [Known limitations](#known-limitations)
- [Roadmap & future improvements](#roadmap--future-improvements)
- [Recognition](#recognition)
- [Cite](#cite)
- [License](#license)

---

## Features
- Real-time **make/miss** classification using **IR** + **vibration** fusion.
- On-device feedback: **RGB LED** + **buzzer**.
- Bluetooth (HC-05/HC-06) Android app displaying **Attempts, Makes, FG%, Timer**, and **session history**.
- **Session comparison**: monitor accuracy and progress over time.
- Simple runtime configuration (`CFG WIN`, `CFG DEB`).
- Fully documented build: circuit, app, enclosure, testing.

---

## Hardware

![BK-Shoot circuit diagram](hardware/images/bkshoot-circuit.png)

> Circuit showing Arduino Uno connected to Bluetooth, IR proximity sensor, vibration sensor, RGB LED, buzzer, and power bank.

See: [`/hardware/bom.md`](hardware/bom.md) and [`/hardware/wiring-table.md`](hardware/wiring-table.md).

---

## How it works
**Sensor fusion logic:**

| Case | Event | Classification |
|------|--------|----------------|
| Ball crosses rim plane (no vibration) | “clean swish” | MAKE |
| Ball crosses rim plane + vibration | “off-rim make” | MAKE |
| Ball vibration only (no rim cross) | “rim-out” | MISS |

> The firmware waits a short period (**FUSE_WINDOW_MS**) after IR detection to check for vibration before classifying.

---

## Quickstart
1. Assemble components per `/hardware/`.
2. Open `/firmware/bk-shoot.ino` in Arduino IDE and upload.
3. Pair Bluetooth: **HC-05**, PIN `1234` or `0000`.
4. Open MIT App Inventor → import `/mobile/bk-shoot.aia` → build `.apk`.
5. Install the app and connect via Bluetooth.

---

## Mobile app
- Displays **Attempts, Makes, Misses, FG%, Timer**.
- Saves session history and allows comparisons.

---

## Testing & validation
Field tests were conducted with multiple players. Data was exported to SQLite and analyzed.

| Metric | Average |
|--------|----------|
| Participants | 20+ |
| Shots/min | 24.5 |
| Mean FG% | 78.2 % |
| Avg. session time | ~75 s |

![Testing setup](testing/01_setup_installation.png)

See all screenshots and analysis: [`/testing/`](testing/)

---

## Known limitations
- **Soft bank shots** may under-vibrate → false miss.  
- **Rim rattles** may trigger false make if IR too permissive.  
- **Bright sunlight** can affect IR readings → shade or use better sensor.

---

## Roadmap & Future Improvements

### Short-term
- Add calibration UI and CSV export to the Android app.  
- Improve vibration filtering.  
- Enhance LED feedback patterns.

### Long-term upgrades
1. **App rewrite in React Native (or Flutter)**  
 - Modern cross-platform app (Android + iOS).  
 - Improved UI/UX, charts, and cloud sync.  
 - Real-time analytics dashboard.

2. **Firmware & hardware upgrade to ESP32 (BLE)**  
 - Replace HC-05 Bluetooth module.  
 - Lower power, faster pairing, OTA updates.

3. **Data & Cloud integration**  
 - Store sessions in SQLite or Firebase.  
 - Web dashboard (React / Next.js) for player analytics.

4. **Machine learning (TinyML)**  
 - On-device model to auto-classify “make/miss” based on vibration and timing.

> Goal: turn BK-Shoot from a working prototype into a professional **smart training platform** for players, coaches, and sport scientists.

---

## Recognition
- Presented at **ICFO Young Photonics Congress (2021)**.

---

## Cite
See [`CITATION.cff`](CITATION.cff).

---

## License
MIT — see [`LICENSE`](LICENSE).
