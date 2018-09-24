#include <Wire.h>
#include <EEPROM.h>
#include <iarduino_OLED_txt.h>
#include "DHT.h"
#include "RTClib.h"

void setup() {
  Serial.begin(9600);
  cfgSetup();
  beepSetup();
  controlSetup();
  oledSetap();
  rtcSetup();
  oledClean();
}

void loop() {
  Serial.println(0);
  controlLoop();
  Serial.println(1);
  menuLoop();
}
