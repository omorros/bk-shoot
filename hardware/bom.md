# Bill of Materials (BOM) — BK-Shoot

This list covers all electronic and mechanical components required to build one BK-Shoot device (Arduino-based basketball shot tracker).

| # | Component | Example model / specs | Qty | Purpose / notes |
|---|------------|----------------------|-----|------------------|
| 1 | **Microcontroller** | Arduino **Uno R3** (or Nano, 5 V) | 1 | Main control board running the firmware |
| 2 | **Distance / proximity sensor** | - Digital IR: **E18-D80NK** (5 V, NPN, 3-wire) <br> - or Analog IR: **Sharp GP2Y0A21YK0F** (10–80 cm) <br> - or Ultrasonic: **HC-SR04** (Trigger/Echo) | 1 | Detects when the ball crosses the rim plane |
| 3 | **Vibration / impact sensor module** | **SW-420 vibration module** (DO output) | 1 | Detects rim/backboard impact to validate makes/misses |
| 4 | **Bluetooth module** | **HC-05** (Classic Bluetooth, 5 V) <br> or **HC-06** | 1 | Sends shot data to the Android app |
| 5 | **RGB LED** | Common cathode or anode RGB LED + 3 × **220 Ω** resistors | 1 | Visual feedback: green = make, red = miss, violet = ready |
| 6 | **Buzzer** | Passive buzzer (5 V, 12 mm) | 1 | Audio feedback for each shot |
| 7 | **Power source** | USB power bank (5 V ≥ 1 A) or Arduino USB cable | 1 | Portable power supply |
| 8 | **Jumper wires / breadboard / connectors** | Dupont male–female cables | ~15 | Internal wiring |
| 9 | **Mounting bracket / enclosure** | 3D-printed case, or plastic holder + zip ties | 1 | To attach the unit safely to the rim or backboard |
| 10 | **Resistors for LED** | 220 Ω (¼ W) | 3 | Current limiters for RGB channels |
| 11 | **Optional components** | Small perfboard or screw terminals | — | For robust wiring or permanent assembly |

---

### Notes & configuration

- **IR / Ultrasonic sensor selection**
  - If your sensor outputs **digital HIGH/LOW** → connect to digital pin D2 (E18-D80NK).
  - If **analog** (Sharp GP2Y0A21) → connect to A0 and set threshold in code.
  - If **ultrasonic** (HC-SR04) → use pins 8 (Trigger) and 7 (Echo); see legacy firmware.

- **Vibration sensor** should be mounted tightly against the rim or backboard so it triggers only on real impacts.

- **Bluetooth HC-05 RX pin** requires a **voltage divider** (1 kΩ + 2 kΩ) to step 5 V → ~3.3 V.

- **LED wiring**
  - If *common cathode*: connect LED “–” to GND.  
  - If *common anode*: connect LED “+” to 5 V and invert PWM logic (already supported in `commonNegative` variable in your legacy code).

- **Power**: use a short, good-quality USB cable; noisy power banks may introduce jitter in analog readings.

---

### Quick setup summary

| Function | Recommended pin | Firmware variable |
|-----------|-----------------|------------------|
| Distance sensor output | D2 (or A0 if analog, 8/7 if ultrasonic) | `PIN_IR_DIGITAL` / `PIN_IR_ANALOG` |
| Vibration sensor DO | D3 | `PIN_VIB` |
| Buzzer | D6 | `PIN_BUZZ` |
| RGB LED | D9 (R), D10 (G), D11 (B) | `PIN_LED_R/G/B` |
| HC-05 Bluetooth | D0 (TX), D1 (RX via divider) | `Serial` interface |