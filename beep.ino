#define BUZZER_PIN 10

void beepSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void beep(unsigned int frequency) {
  DateTime now = rtcNow();
  int nowHM = now.hour() * 60 + now.minute();
  if (cfgGetSound() && !rtcBetweenSerially(nowHM, cfgGetSoundStartHM(), cfgGetSoundEndHM())) {
    tone(BUZZER_PIN, frequency, 100);
  }
}

