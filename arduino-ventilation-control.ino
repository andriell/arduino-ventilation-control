#include <EEPROM.h>
#include <iarduino_OLED_txt.h>
#include <iarduino_RTC.h>
#include "DHT.h"

void setup() {
  Serial.begin(9600);
  cfgSetup();
  beepSetup();
  controlSetup();
  oledSetap();
  clockSetup();
  oledClean();
}

void loop() {
  controlLoop();
  cfgLoop();
  //clockLoop();
}
