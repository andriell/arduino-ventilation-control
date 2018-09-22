const int BUZZER_PIN = 10;

void beepSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
  //tone(BUZZER_PIN, 2000, 100);
}

void beep(unsigned int frequency) {
  if (cfgGetSound() > 0) {
    tone(BUZZER_PIN, frequency, 100);
  }
}

