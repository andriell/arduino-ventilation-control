const int J_X_PIN = A1;
const int J_Y_PIN = A0;

// -- Начало Джойстик
unsigned int controlButtons = 0;
// -- Конец Джойстик

// 10 - вверх +
// 11 - вправо Enter
// 12 - вниз - 
// 13 - влево Cancel

void controlSetup() {
  pinMode(J_X_PIN, INPUT);
  pinMode(J_Y_PIN, INPUT);
}

void controlLoop(byte t) {
  int s;
  for (byte i = 0; i < t; i++) {
    if (i > 0) {
      delay(100);
    }
    s = analogRead(J_X_PIN);
    if (s < 256) {
      bitSet(controlButtons, 12);
      beep(100);
      break;
    } else if (s > 768) {
      bitSet(controlButtons, 10);
      beep(120);
      break;
    }
    s = analogRead(J_Y_PIN);
    if (s < 256) {
      bitSet(controlButtons, 13);
      beep(140);
      break;
    } else if (s > 768) {
      bitSet(controlButtons, 11);
      beep(160);
      break;
    }
  }
}

bool controlRead(int i) {
  return bitRead(controlButtons, i);
}
// вверх +
bool controlP() {
  return bitRead(controlButtons, 10);
}
// вниз - 
bool controlM() {
  return bitRead(controlButtons, 12);
}
// вправо Enter
bool controlE() {
  return bitRead(controlButtons, 11);
}
// влево Cancel
bool controlC() {
  return bitRead(controlButtons, 13);
}

