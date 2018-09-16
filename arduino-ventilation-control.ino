#include <iarduino_OLED_txt.h>
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  cfgSetup();
  beepSetup();
  controlSetup();
  oledSetap();
}

void loop() {
  controlLoop();
  cfgLoop();
}
