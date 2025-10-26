# Bill of Materials (BOM)

| # | Component | Model / specs | Qty | Purpose |
|---|------------|----------------|-----|----------|
| 1 | Arduino Uno (5V) | Any R3/Nano | 1 | Main controller |
| 2 | IR proximity sensor | E18-D80NK or Sharp GP2Y0A21 | 1 | Detects ball crossing |
| 3 | Vibration sensor | SW-420 module | 1 | Detects rim/backboard impact |
| 4 | Bluetooth module | HC-05 or HC-06 | 1 | Sends data to phone |
| 5 | RGB LED + resistors | 3 × 220Ω | 1 | Visual feedback |
| 6 | Passive buzzer | 5V, 12 mm | 1 | Audio feedback |
| 7 | Power supply | USB 5V power bank | 1 | Portable power |
| 8 | Wiring | Dupont cables / breadboard | ~15 | Connections |
| 9 | Mount | Plastic/3D-printed box + ties | 1 | Hoop/backboard mount |

**Notes**
- HC-05 RX needs a 5V→3.3V divider.  
- LED common cathode/anode supported (adjust code if needed).  
- Optional upgrade: use ESP32 for BLE and onboard power regulation.
