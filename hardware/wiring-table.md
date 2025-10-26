# Wiring Table

| Function | Module Pin | Arduino Pin | Notes |
|-----------|-------------|-------------|-------|
| IR sensor | OUT | D2 | Ball crossing detection |
| Vibration sensor | DO | D3 | Detects rim impact |
| RGB LED R | R via 220Ω | D9 | PWM |
| RGB LED G | G via 220Ω | D10 | PWM |
| RGB LED B | B via 220Ω | D11 | PWM |
| Buzzer | + | D6 | Tone feedback |
| HC-05 TX | TXD | D0 | Into Arduino RX |
| HC-05 RX | RXD (via divider) | D1 | Arduino TX (5V → 3.3V) |
| +5V | VCC | 5V | Power supply |
| GND | GND | GND | Common ground |

---

### Mounting
- Align IR horizontally across rim plane.  
- Fix vibration sensor tightly to rim base.  
- Enclosure mounted behind backboard, USB-powered.
