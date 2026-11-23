/*======================================================
                    DIGITAL OUTPUT
========================================================
Description:
Controls 6 LEDs using 2 toggle switches (using standard 
INPUT with external pull-downs) OR a Web Serial browser 
GUI over USB. Displays active mode on an LCD1602A I2C display.

Designer Engineer:
Ebio, Ann Clarise
Jaena, Elaiza B.

Date:
Nov. 18 2026
======================================================*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// GPIO
const int LEDS[] = {32, 33, 25, 0, 2, 15};
const int NUM_PINS = sizeof(LEDS) / sizeof(LEDS[0]);
const unsigned long DELAY_TIME = 500;

// Pins
const int SW1_PIN = 26;
const int SW2_PIN = 27;

const int SDA_PIN = 21;
const int SCL_PIN = 22;

#include "LED_Modes.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

int currentMode = 0;
int lastDisplayedMode = -1;
int lastSwitchState = -1;
String controlSource = "SWITCH";

void updateLCD(int mode, String source) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SRC: ");
  lcd.print(source);

  lcd.setCursor(0, 1);
  lcd.print("MODE: ");
  switch (mode) {
    case 0: lcd.print("OFF (00)"); break;
    case 1: lcd.print("BLINK (10)"); break;
    case 2: lcd.print("ALT (01)"); break;
    case 3: lcd.print("RUN (11)"); break;
  }
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

  pinMode(SW1_PIN, INPUT);
  pinMode(SW2_PIN, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  lcd.init();
  lcd.backlight();
  updateLCD(currentMode, controlSource);
}

void loop() {
  int s1 = digitalRead(SW1_PIN);
  int s2 = digitalRead(SW2_PIN);
  int switchState = (s1 << 1) | s2;

  if (switchState != lastSwitchState) {
    lastSwitchState = switchState;
    controlSource = "SWITCH";
    switch (switchState) {
      case 0b00: currentMode = 0; break; // 0 0 = off
      case 0b10: currentMode = 1; break; // 1 0 = blink
      case 0b01: currentMode = 2; break; // 0 1 = alt
      case 0b11: currentMode = 3; break; // 1 1 = run
    }
  }

  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd >= '0' && cmd <= '3') {
      currentMode = cmd - '0';
      controlSource = "USB WEB";
      lastDisplayedMode = -1;
    }
  }

  if (currentMode != lastDisplayedMode) {
    resetAnimation();
    updateLCD(currentMode, controlSource);
    lastDisplayedMode = currentMode;
  }

  switch (currentMode) {
    case 0: modeOff(); break;
    case 1: blink();   break;
    case 2: alt();     break;
    case 3: run();     break;
  }
}