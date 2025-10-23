#include <Arduino.h>

/*
  BK-Shoot — Arduino firmware (v1.0)
  Classifies basketball shots (make/miss) using IR + vibration sensor fusion.
  - IR = ball crosses the rim plane
  - Vibration = rim/backboard impact
  Decision rule: if IR triggers, then vibration within a time window => MAKE, else MISS.

  Runtime commands over Serial/Bluetooth (9600 baud):
    CFG?             -> prints current config
    CFG WIN=<ms>     -> sets FUSE_WINDOW_MS (e.g., 800..1500)
    CFG DEB=<ms>     -> sets DEBOUNCE_MS (e.g., 150..500)
    PING             -> returns 'PONG'

  Output event format (line-delimited JSON):
    {"type":"make","ts":1234567}
    {"type":"miss","ts":1237890}

  Hardware defaults assume:
    - IR proximity: E18-D80NK (digital output) on D2
      (If using analog IR like GP2Y0A21, see ANALOG_IR section below)
    - Vibration module SW-420 on D3 (digital)
    - RGB LED on D9/D10/D11 (with 220Ω resistors)
    - Buzzer on D6
    - HC-05 Bluetooth wired to pins D0/D1 (hardware Serial) or left paired via USB Serial to PC
      (If wiring HC-05 RX to an Arduino TX pin, use a voltage divider to drop 5V -> ~3.3V on HC-05 RX)
*/

////////////////////
// Pin assignment //
////////////////////
#define PIN_IR_DIGITAL  2   // digital IR sensor output (E18-D80NK). HIGH when object detected.
#define PIN_VIB         3   // digital vibration module (SW-420). HIGH on impact.
#define PIN_BUZZ        6
#define PIN_LED_R       9
#define PIN_LED_G       10
#define PIN_LED_B       11

// If you use an analog IR like GP2Y0A21, uncomment the next line and wire sensor to A0.
// #define USE_ANALOG_IR
#ifdef USE_ANALOG_IR
  #define PIN_IR_ANALOG A0
  // Tune this threshold based on your mounting; print analog values during dry runs.
  int IR_ANALOG_THRESHOLD = 320; // ~0..1023 (lower/higher depending on distance & model)
#endif

////////////////////
// Tuning values  //
////////////////////
unsigned long FUSE_WINDOW_MS = 1000; // window to accept vibration after IR trigger (typ: 800..1200)
unsigned long DEBOUNCE_MS    = 250;  // minimum gap between classified shots (typ: 200..400)

// Internal state
unsigned long lastEventMs = 0;
bool irTriggered = false;
unsigned long irTimeMs = 0;
String rxBuf;

////////////////////
// LED helpers    //
////////////////////
inline void ledColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(PIN_LED_R, r);
  analogWrite(PIN_LED_G, g);
  analogWrite(PIN_LED_B, b);
}
inline void ledOff() { ledColor(0,0,0); }

////////////////////
// Setup / Loop   //
////////////////////
void setup() {
#ifndef USE_ANALOG_IR
  pinMode(PIN_IR_DIGITAL, INPUT);
#else
  pinMode(PIN_IR_ANALOG, INPUT);
#endif
  pinMode(PIN_VIB, INPUT);
  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  Serial.begin(9600);
  delay(150);
  printBanner();
  printConfig();

  // Startup blink
  ledColor(0, 40, 120); delay(250); ledOff();
}

void loop() {
  const unsigned long now = millis();

  // Handle config commands typed from Serial/BT
  readCommands();

  // Global debounce: avoid double-classifying same shot
  if (now - lastEventMs < DEBOUNCE_MS) return;

  // 1) Detect IR "crossing" (ball through rim plane)
  if (!irTriggered && irActive()) {
    irTriggered = true;
    irTimeMs = now;
  }

  // 2) After IR, look for vibration within window => MAKE
  if (irTriggered && (now - irTimeMs) <= FUSE_WINDOW_MS) {
    if (vibrationActive()) {
      classifyMake(now);
      return; // early exit after classification
    }
  }

  // 3) If window elapsed without vibration => MISS
  if (irTriggered && (now - irTimeMs) > FUSE_WINDOW_MS) {
    classifyMiss(now);
    return;
  }
}

////////////////////
// Sensors        //
////////////////////
bool irActive() {
#ifndef USE_ANALOG_IR
  // Digital IR (E18-D80NK): HIGH = object detected
  return digitalRead(PIN_IR_DIGITAL) == HIGH;
#else
  // Analog IR (GP2Y0A21 etc.): compare against threshold
  int val = analogRead(PIN_IR_ANALOG);
  // Uncomment to debug:
  // Serial.print("IR="); Serial.println(val);
  return val >= IR_ANALOG_THRESHOLD;
#endif
}

bool vibrationActive() {
  return digitalRead(PIN_VIB) == HIGH;
}

////////////////////
// Classification //
////////////////////
void classifyMake(unsigned long now) {
  feedbackMake();
  emitEvent("make", now);
  resetState(now);
}

void classifyMiss(unsigned long now) {
  feedbackMiss();
  emitEvent("miss", now);
  resetState(now);
}

void resetState(unsigned long now) {
  irTriggered = false;
  lastEventMs = now;
}

////////////////////
// Feedback       //
////////////////////
void feedbackMake() {
  // Green flash + short high tone
  ledColor(0, 255, 0);
  tone(PIN_BUZZ, 2000, 120);
  delay(140);
  ledOff();
}

void feedbackMiss() {
  // Red flash + short lower tone
  ledColor(255, 0, 0);
  tone(PIN_BUZZ, 900, 120);
  delay(140);
  ledOff();
}

////////////////////
// Messaging      //
////////////////////
void emitEvent(const char* type, unsigned long ts) {
  Serial.print("{\"type\":\""); Serial.print(type);
  Serial.print("\",\"ts\":"); Serial.print(ts);
  Serial.println("}");
}

////////////////////
// Commands       //
////////////////////
void readCommands() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') {
      if (rxBuf.length() > 0) {
        handleCommand(rxBuf);
        rxBuf = "";
      }
    } else {
      rxBuf += c;
    }
  }
}

void handleCommand(String s) {
  s.trim();
  if (s.length() == 0) return;

  // Make command parsing case-insensitive
  String S = s; S.toUpperCase();

  if (S == "CFG?" || S == "CFG") {
    printConfig();
    return;
  }
  if (S.startsWith("CFG WIN=")) {
    long v = S.substring(8).toInt();
    if (v >= 200 && v <= 3000) {
      FUSE_WINDOW_MS = (unsigned long)v;
      Serial.println("OK WIN");
    } else {
      Serial.println("ERR WIN range 200..3000");
    }
    printConfig();
    return;
  }
  if (S.startsWith("CFG DEB=")) {
    long v = S.substring(8).toInt();
    if (v >= 100 && v <= 2000) {
      DEBOUNCE_MS = (unsigned long)v;
      Serial.println("OK DEB");
    } else {
      Serial.println("ERR DEB range 100..2000");
    }
    printConfig();
    return;
  }
#ifdef USE_ANALOG_IR
  if (S.startsWith("CFG IRTHR=")) {
    long v = S.substring(10).toInt();
    if (v >= 50 && v <= 900) {
      IR_ANALOG_THRESHOLD = (int)v;
      Serial.println("OK IRTHR");
    } else {
      Serial.println("ERR IRTHR range 50..900");
    }
    printConfig();
    return;
  }
#endif
  if (S == "PING") {
    Serial.println("PONG");
    return;
  }

  Serial.print("ERR Unknown cmd: ");
  Serial.println(s);
  Serial.println("Use: CFG?, CFG WIN=<ms>, CFG DEB=<ms>"
#ifdef USE_ANALOG_IR
                 ", CFG IRTHR=<0..1023>"
#endif
  );
}

void printConfig() {
  Serial.print("CFG WIN="); Serial.print(FUSE_WINDOW_MS);
  Serial.print(" DEB=");     Serial.print(DEBOUNCE_MS);
#ifdef USE_ANALOG_IR
  Serial.print(" IRTHR=");   Serial.print(IR_ANALOG_THRESHOLD);
#endif
  Serial.println();
}

void printBanner() {
  Serial.println(F("BK-Shoot firmware v1.0 — 9600 baud"));
  Serial.println(F("Commands: CFG? | CFG WIN=<ms> | CFG DEB=<ms> | PING"
#ifdef USE_ANALOG_IR
                   " | CFG IRTHR=<0..1023>"
#endif
  ));
}
