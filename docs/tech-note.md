# BK-Shoot — Technical Note

**Author:** Oriol Morros Vilaseca  
**Version:** 1.0  

---

## 1. Problem
Develop a low-cost, accurate basketball shot-tracking system using accessible hardware and an educational development platform.

---

## 2. Methodology
- **Hardware:** Arduino Uno, IR sensor (E18-D80NK), vibration sensor (SW-420), RGB LED, buzzer, HC-05 Bluetooth.
- **Firmware:** Sensor fusion — IR triggers, waits up to FUSE_WINDOW_MS for vibration to classify make/miss.
- **Software:** Android app (MIT App Inventor) receives JSON lines, displays real-time stats, and stores session history.

---

## 3. Hardware design
Circuit wiring shown in `/hardware/images/bkshoot-circuit.png`.  
All parts powered by 5V power bank. LED and buzzer provide instant feedback.

---

## 4. Validation
Field-tested with ~20 players, ~2000 total shots.

| Metric | Average |
|--------|----------|
| Accuracy (FG%) | 78.2 % |
| Shots/min | 24.5 |
| Mean latency | ~250 ms |
| Power runtime | >3 h (USB bank) |

See `/testing/` for setup and analysis screenshots.

---

## 5. Discussion
- Effective detection under controlled lighting.  
- Occasional false misses on soft bank shots.  
- Sensor alignment critical for precision.  

---

## 6. Limitations
- IR interference in bright sunlight.  
- Limited Bluetooth range (~8–10 m).  
- App Inventor UI constraints for scaling.

---

## 7. Future improvements
1. **Rewrite app in React Native or Flutter**  
   - Cross-platform, modern UI, integrated analytics.

2. **Switch to ESP32 (BLE)**  
   - Eliminate HC-05, add BLE and Wi-Fi support.

3. **Enhanced data pipeline**  
   - Store sessions in Firebase or SQLite.  
   - Add cloud dashboard (React / Next.js).

4. **Machine learning**  
   - Apply TinyML for auto-classification of makes/misses.

5. **Extended hardware**  
   - Add IMU or camera module for motion capture.

---

## 8. Conclusion
BK-Shoot proves the feasibility of affordable, real-time sports analytics using open hardware and educational tools.  
Future versions will migrate toward professional-grade platforms while keeping the core educational spirit.
