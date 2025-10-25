# BK-Shoot — Technical Note

**Author:** Oriol Morros Vilaseca   |   **Version:** 1.0   |   **Date:** 2025-10-24

## 1 Problem
Accurately detect basketball shots (make/miss) using low-cost sensors.

## 2 Approach
IR (E18-D80NK) + vibration (SW-420) fusion.  
LED + buzzer feedback.  
Bluetooth → MIT App Inventor app.

## 3 Validation
| True / Pred. | Make | Miss |
|---------------|------|------|
| Make | TP | FN |
| Miss | FP | TN |
Accuracy ≈ 90 %.  Latency ≈ 200–300 ms.

## 4 Next steps
App calibration UI, BLE/ESP32 version, TinyML upgrade.

## 5 Recognition
ICFO Young Photonics Congress (2021).