// -- Начало Джойстик
byte controlButtons = 0;
// -- Конец Джойстик

// 10 - вверх +
// 11 - вправо Enter
// 12 - вниз - 
// 13 - влево Cancel

void controlSetup() {
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
}

void controlLoop() {
  controlButtons = 0;
  for (byte i = 0; i < 5; i++) {
    if (i > 0) {
      delay(100);
    }
    if (!digitalRead(A0)) {
      bitSet(controlButtons, 0);
      beep(200);
      break;
    }
    if (!digitalRead(A1)) {
      bitSet(controlButtons, 1);
      beep(400);
      break;
    }
    if (!digitalRead(A2)) {
      bitSet(controlButtons, 2);
      beep(600);
      break;
    }
    if (!digitalRead(A3)) {
      bitSet(controlButtons, 3);
      beep(800);
      break;
    }
  }
  if (controlButtons != 0) {
    delay(200);
  }
}

void controlReset() {
  controlButtons = 0;
}
// +
bool controlP() {
  return bitRead(controlButtons, 0);
}
// - 
bool controlM() {
  return bitRead(controlButtons, 1);
}
// Enter
bool controlE() {
  return bitRead(controlButtons, 3);
}
// Cancel
bool controlC() {
  return bitRead(controlButtons, 2);
}

