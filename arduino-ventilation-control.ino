#include <iarduino_OLED_txt.h> 

void setup() {
  Serial.begin(9600);
  beepSetup();
  controlSetup();
  oledSetap();
}

void loop() {
  controlLoop(5);
}
