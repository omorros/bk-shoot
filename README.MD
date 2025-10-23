# BK-Shoot — Arduino + MIT App Inventor basketball shot tracker

A low-cost hoop-mounted gadget + Android app that detects **makes/misses** in real time and tracks shooting stats. Built with **Arduino** sensors (IR + vibration) and a **Bluetooth** link to a MIT App Inventor app.

> Status: **v1.0 (public release)** · License: **MIT** · Languages: **EN / ES**


## Demo
_Add a 10–20 s GIF/MP4 here showing the device classifying shots and the app updating counts._

---

## Features
- Real time **make/miss** classification using **IR** (ball crossing rim plane) + **vibration** (impact at rim/backboard).
- On device feedback: **RGB LED** + **buzzer**.
- Bluetooth (HC-05/HC-06) Android app that shows: **Attempts, Makes, FG%, Session timer**, and **per session history**.
- **Session comparison**: review previous sessions and track progress over time (FG% trend, attempts/min).
- One file firmware with simple **on court calibration commands**.
- Clear documentation: BOM, wiring tables, validation protocol, and data schema.

---

## Hardware
- Microcontroller: **Arduino Uno/Nano (5V)**
- Sensors: **IR proximity** (E18-D80NK / GP2Y0A21) + **vibration module** (SW-420)
- Comms: **HC-05 / HC-06** Bluetooth
- Output: **RGB LED** (+ 3×220 Ω resistors), **passive buzzer**
- Power: **5 V** (USB power bank)
- Mounting: Simple bracket/enclosure (see `/hardware`)

See: [`/hardware/bom.md`](hardware/bom.md) and [`/hardware/wiring-table.md`](hardware/wiring-table.md).

---

## How it works
**Sensor fusion:** when the IR sensor triggers (ball crosses the rim plane), firmware opens a short time window (**FUSE_WINDOW_MS**) to check for vibration. The three practical outcomes are:

1. **Ball goes in with no vibration** → **MAKE** (clean swish; IR triggers, no impact).
2. **Ball touches rim/backboard and goes in with vibration** → **MAKE** (IR + vibration within window).
3. **Ball impacts rim/backboard and goes out** → **MISS** (IR triggers; vibration occurs, but logic classifies overall as miss because the ball doesn’t clear as a valid make; see Notes below).

> **Notes:** Physical setups vary. Some misses will still produce an IR trigger without a successful make; the fusion window and debounce help disambiguate. Tune `FUSE_WINDOW_MS` and mounting to minimize edge cases (e.g., long rattles or soft bank shots).

---

## Quickstart
1. **Assemble hardware** per `/hardware`.
2. **Flash firmware**: open [`/firmware/bk-shoot.ino`](firmware/bk-shoot.ino) in Arduino IDE → select board/port → **Upload**.
3. **Pair Bluetooth**: phone → Bluetooth → pair **HC-05** (PIN `1234` or `0000`).
4. **Install app**:
   - Import [`/mobile/bk-shoot.aia`](mobile/bk-shoot.aia) into [MIT App Inventor](https://appinventor.mit.edu/).
   - Build `.apk`, install on your Android phone.
5. **Calibrate**:
   - Mount the IR so it trips only when the ball fully crosses the rim plane.
   - Start with `FUSE_WINDOW_MS = 1000` and `DEBOUNCE_MS = 250`.
   - Use the serial/Bluetooth commands below to tune without reflashing.
6. **Test 10–20 shots** and adjust the window if needed.