#define FAN1_PIN 5
#define FAN2_PIN 6

struct FanStruct {
  int pin;
  unsigned long stopMicros;
  unsigned long rpmMicros;
  int rpm;
  unsigned long lastWorkDay;
  int lastWorkDaySec;
};

FanStruct funList[] = {
  {FAN1_PIN, 0, 0, 0, 0, 0},
  {FAN2_PIN, 0, 0, 0, 0, 0},
};

void fanSetup() {
  pinMode(FAN1_PIN, OUTPUT);
  pinMode(FAN2_PIN, OUTPUT);
  attachInterrupt(0, fanSens1, RISING);
  attachInterrupt(1, fanSens2, RISING);
}

void fanLoop() {
  for (byte b = 0; b < 2; b++) {
    if ((micros() - funList[b].rpmMicros) > ONE_SECOND) {
      funList[b].rpm = 0;
    }
    if (funList[b].stopMicros < micros()) {
      analogWrite(funList[b].pin, 0);
    }
  }
}

void funRun(byte fan, int sec) {
  if (funList[fan].stopMicros < micros()) {
    funList[fan].stopMicros = micros();
  }
  funList[fan].stopMicros += ((unsigned long) sec) * ONE_SECOND;
  if (funList[fan].lastWorkDay != funDayId()) {
    funList[fan].lastWorkDaySec = 0;
  }
  funList[fan].lastWorkDaySec += sec;
  if (fan == 0) {
    analogWrite(funList[fan].pin, cfgGetSpeedFan0());
  } else {
    analogWrite(funList[fan].pin, cfgGetSpeedFan1());
  }
}

void funStop(byte fan) {
  funList[fan].stopMicros = micros();
}

void fanSens1() {
  funList[1].rpm = 60 / ((float) (micros() - funList[1].rpmMicros) / ONE_SECOND);
  funList[1].rpmMicros = micros();
}

void fanSens2() {
  funList[1].rpm = 60 / ((float) (micros() - funList[1].rpmMicros) / ONE_SECOND);
  funList[1].rpmMicros = micros();
}

int funRpm(byte fan) {
  return funList[fan].rpm;
}

// Сколько секунд еще работать
int funSec(byte fan) {
  int r = (funList[fan].stopMicros - micros()) / ONE_SECOND;
  if (r < 0) {
    return 0;
  }
  return r;
}

// Скорость вращения вентилятора. 8 символов
String funRpmStr(byte fan) {
  sprintf(char22, "%5drpm", funList[fan].rpm);
  char22[8] = '\0';
  return char22;
}

// Сколько вентилятору еще работать. 8 символов
String funSecStr(byte fan) {
  int val = funSec(fan);
  sprintf(char22, "%02d:%02d:%02d", val / 3600, (val % 3600) / 60, val % 60);
  char22[8] = '\0';
  return char22;
}

// Последний рабочий день. 10 символов
String funLastWorkDayStr(byte fan) {
  unsigned long val = funList[fan].lastWorkDay;
  sprintf(char22, "%04d-%02d-%02d", val / 10000ul, (val % 10000ul) / 100, val % 100ul);
  char22[10] = '\0';
  return char22;
}

// Сколько времени кулер работал в этот день. 8 символов
String funLastWorkDaySecStr(byte fan) {
  unsigned long val = funList[fan].lastWorkDaySec;
  sprintf(char22, "%02d:%02d:%02d", val / 3600, (val % 3600) / 60, val % 60);
  char22[8] = '\0';
  return char22;
}

// Уникальный идентификатор дня в календаре начиная с 2000 года
unsigned long funDayId() {
  DateTime now = rtcNow();
  return ((unsigned long) now.year()) * 10000ul  + ((unsigned long) now.month()) * 100ul + ((unsigned long) now.day());
}

