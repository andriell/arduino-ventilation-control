#define FAN1_PIN 5
#define FAN2_PIN 6

struct FanStruct {
  int pin;
  unsigned long stopSec;
  unsigned long rpmMicros;
  int rpm;
  unsigned long lastWorkDay;
  unsigned long lastWorkDaySec;
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
    if (funList[b].stopSec < micros() / ONE_SECOND) {
      analogWrite(funList[b].pin, 0);
    }
  }
}

void funRun(byte fan, unsigned long sec) {
  unsigned long m = micros() / ONE_SECOND;
  if (funList[fan].stopSec < m) {
    funList[fan].stopSec = m;
  }
  if (funList[fan].stopSec + sec - m > 3600ul) {
    sec = 3600ul - (funList[fan].stopSec - m);
  }
  funList[fan].stopSec += sec;
  if (funList[fan].lastWorkDay != funDayId()) {
    funList[fan].lastWorkDay = funDayId();
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
  unsigned long m = micros() / ONE_SECOND;
  if (funList[fan].stopSec > m) {
    funList[fan].lastWorkDaySec -= (funList[fan].stopSec - m);
    funList[fan].stopSec = m;
  }
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
  unsigned long sec = micros() / ONE_SECOND;
  if (sec > funList[fan].stopSec) {
    return 0;
  }
  return funList[fan].stopSec - sec;
}

// Скорость вращения вентилятора. 8 символов
char* funRpmStr(byte fan) {
  sprintf(char22, "%5drpm\0", funList[fan].rpm);
  char22[8] = '\0';
  return char22;
}

// Сколько вентилятору еще работать. 5 символов
char* funSecStr(byte fan) {
  int val = funSec(fan);
  sprintf(char22, "%02d:%02d\0", val / 60, val % 60);
  char22[5] = '\0';
  return char22;
}

// Последний рабочий день. 10 символов
char* funLastWorkDayStr(byte fan) {
  int y = funList[fan].lastWorkDay / 10000ul;
  int m = (funList[fan].lastWorkDay - y * 10000ul) / 100;
  int d = funList[fan].lastWorkDay - y * 10000ul - m * 100;
  sprintf(char22, "%04d-%02d-%02d\0", y, m, d);
  char22[10] = '\0';
  return char22;
}

// Сколько времени кулер работал в этот день. 8 символов
char* funLastWorkDaySecStr(byte fan) {
  int h = funList[fan].lastWorkDaySec / 3600;
  int m = (funList[fan].lastWorkDaySec - h * 3600) / 60;
  int s = funList[fan].lastWorkDaySec - h * 3600 - m * 60;
  sprintf(char22, "%02d:%02d:%02d\0", h, m, s);
  char22[8] = '\0';
  return char22;
}

// Уникальный идентификатор дня в календаре начиная с 2000 года
unsigned long funDayId() {
  DateTime now = rtcNow();
  return ((unsigned long) now.year()) * 10000ul  + ((unsigned long) now.month()) * 100ul + ((unsigned long) now.day());
}

