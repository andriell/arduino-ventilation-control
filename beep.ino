#define BUZZER_PIN 10

void beepSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void beep(unsigned int frequency) {
  int nowHM = rtcHm();
  if (cfgGetSound() && !rtcBetweenSerially(nowHM, cfgGetSoundStartHM(), cfgGetSoundEndHM())) {
    tone(BUZZER_PIN, frequency, 100);
  }
}

