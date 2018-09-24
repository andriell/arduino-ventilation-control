#include <EEPROM.h>
#include <iarduino_OLED_txt.h>
#include "DHT.h"
#include "RTClib.h"

#define ONE_SECOND 1000000ul

char char22[22];

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
  controlLoop();
  menuLoop();
  fanLoop();
}
