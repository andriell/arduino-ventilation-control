#define FAN1_PIN 5
#define FAN2_PIN 6
#define ONE_SECOND 1000000ul

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
  unsigned long m = micros();
  for (byte b = 0; b < 2; b++) {
    if (funList[b].rpmMicros > m) {
      funList[b].rpmMicros = m;
      continue;
    }
    if ((m - funList[b].rpmMicros) > ONE_SECOND) {
      funList[b].rpm = 0;
    }
    if (funList[b].stopSec < rtcUnixtime()) {
      analogWrite(funList[b].pin, 0);
    }
  }
}

void funRunAll(unsigned long stopUnixtime) {
  funRun(0, stopUnixtime);
  funRun(1, stopUnixtime);
}

void funRun(byte fan, unsigned long stopUnixtime) {
  unsigned long ut = rtcUnixtime();
  if (stopUnixtime - ut > 3600ul) {
    stopUnixtime = ut + 3600ul;
  }
  if (funList[fan].lastWorkDay != funDayId()) {
    funList[fan].lastWorkDay = funDayId();
    funList[fan].lastWorkDaySec = 0;
  }
  if (funList[fan].stopSec > ut) {
    // Кулер работает
    funList[fan].lastWorkDaySec -= funList[fan].stopSec - ut;
  }
  funList[fan].lastWorkDaySec += stopUnixtime - ut;

  funList[fan].stopSec = stopUnixtime;
  if (fan == 0) {
    analogWrite(funList[fan].pin, cfgGetSpeedFan0());
  } else {
    analogWrite(funList[fan].pin, cfgGetSpeedFan1());
  }
}
void funStopAll() {
  funStop(0);
  funStop(1);
}

void funStop(byte fan) {
  unsigned long ut = rtcUnixtime();
  if (funList[fan].stopSec > ut) {
    funList[fan].lastWorkDaySec -= (funList[fan].stopSec - ut);
    funList[fan].stopSec = ut;
  }
}

void fanSens1() {
  unsigned long m = micros();
  if (funList[1].rpmMicros > m) {
    funList[1].rpmMicros = m;
    return;
  }
  funList[1].rpm = 60 / (((float) (m - funList[1].rpmMicros)) / ONE_SECOND);
  funList[1].rpmMicros = m;
}

void fanSens2() {
  unsigned long m = micros();
  if (funList[2].rpmMicros > m) {
    funList[2].rpmMicros = m;
    return;
  }
  funList[2].rpm = 60 / (((float) (m - funList[2].rpmMicros)) / ONE_SECOND);
  funList[2].rpmMicros = m;
}

int funRpm(byte fan) {
  return funList[fan].rpm;
}

// Сколько секунд еще работать
int funRunSecMin() {
  return min(funRunSec(0), funRunSec(1));
}

// Сколько секунд еще работать
int funRunSec(byte fan) {
  unsigned long ut = rtcUnixtime();
  if (ut > funList[fan].stopSec) {
    return 0;
  }
  return funList[fan].stopSec - ut;
}

bool funIsRunAll() {
  unsigned long ut = rtcUnixtime();
  return funList[0].stopSec > ut && funList[1].stopSec > ut;
}

bool funIsRun(byte fan) {
  return funList[fan].stopSec > rtcUnixtime();
}

// Скорость вращения вентилятора. 8 символов
char* funRpmStr(byte fan) {
  sprintf(char22, "%5drpm\0", funList[fan].rpm);
  char22[8] = '\0';
  return char22;
}

// Сколько вентилятору еще работать. 5 символов
char* funSecStr(byte fan) {
  int val = funRunSec(fan);
  sprintf(char22, "%02d:%02d\0", val / 60, val % 60);
  char22[5] = '\0';
  return char22;
}
// Сколько секунд кулер не работал
unsigned long funTimeOutOfWorkMax() {
  return max(funTimeOutOfWork(0), funTimeOutOfWork(1));
}

// Сколько секунд кулер не работал
unsigned long funTimeOutOfWork(byte b) {
  unsigned long ut = rtcUnixtime();
  if (funList[b].stopSec > ut) {
    return 0ul;
  }
  return ut - funList[b].stopSec;
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

