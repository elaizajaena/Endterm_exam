LED_Modes.h

extern const int LEDS[];
extern const int NUM_PINS;
extern const unsigned long DELAY_TIME;

unsigned long prevMillis = 0;
int animStep = 0;
int animDir = 1;

void setAllLEDs(uint8_t state) {
  for (int i = 0; i < NUM_PINS; i++) {
    digitalWrite(LEDS[i], state);
  }
}

void resetAnimation() {
  animStep = 0;
  animDir = 1;
  prevMillis = 0;
  setAllLEDs(LOW);
}

void modeOff() {
  setAllLEDs(LOW);
}

void blink() {
  unsigned long now = millis();
  if (now - prevMillis >= DELAY_TIME) {
    prevMillis = now;
    setAllLEDs(animStep % 2 == 0 ? HIGH : LOW);
    animStep++;
  }
}

void alt() {
  unsigned long now = millis();
  if (now - prevMillis >= DELAY_TIME) {
    prevMillis = now;
    bool firstHalf = (animStep % 2 == 0);
    for (int j = 0; j < NUM_PINS; j++) {
      if (firstHalf) {
        digitalWrite(LEDS[j], j < 3 ? HIGH : LOW);
      } else {
        digitalWrite(LEDS[j], j < 3 ? LOW : HIGH);
      }
    }
    animStep++;
  }
}

void run() {
  unsigned long now = millis();
  if (now - prevMillis >= DELAY_TIME) {
    prevMillis = now;
    setAllLEDs(LOW);
    digitalWrite(LEDS[animStep], HIGH);

    animStep += animDir;
    if (animStep >= NUM_PINS - 1) {
      animStep = NUM_PINS - 1;
      animDir = -1;
    } else if (animStep <= 0) {
      animStep = 0;
      animDir = 1;
    }
  }
}