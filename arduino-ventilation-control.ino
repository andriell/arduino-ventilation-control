#include <EEPROM.h>
#include <iarduino_OLED_txt.h>
#include "DHT.h"
#include "RTClib.h"

void setup() {
  // Serial.begin(9600);
  cfgSetup();
  rtcSetup();
  beepSetup();
  controlSetup();
  fanSetup();
  oledSetap();
  delay(200);
  oledClean();
}

void loop() {
  rtcLoop();
  controlLoop();
  dthLoop();
  fanLoop();
  modLoop();
  logLoop();
  menuLoop();
}
