#define FAN1_PIN_HZ 2
#define FAN2_PIN_HZ 3
#define FAN1_PIN 5
#define FAN2_PIN 6
#define ONE_SECOND 1000000ul

struct FanStruct {
  int pin;
  unsigned long stopSec;
  unsigned long rpmMicros;
  unsigned long rpmCount;
  long rpm;
  unsigned long lastWorkDay;
  unsigned long lastWorkDaySec;
};

FanStruct fanList[] = {
  {FAN1_PIN, 0, 0, 0, 0, 0},
  {FAN2_PIN, 0, 0, 0, 0, 0},
};

void fanSetup() {
  pinMode(FAN1_PIN, OUTPUT);
  pinMode(FAN2_PIN, OUTPUT);
  analogWrite(FAN1_PIN, 0);
  analogWrite(FAN2_PIN, 0);
  pinMode(FAN1_PIN_HZ, INPUT_PULLUP);
  pinMode(FAN2_PIN_HZ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FAN1_PIN_HZ), fanSens1, FALLING);
  attachInterrupt(digitalPinToInterrupt(FAN2_PIN_HZ), fanSens2, FALLING);
}

void fanLoop() {
  unsigned long m = micros();
  for (byte b = 0; b < 2; b++) {
    if (fanList[b].rpmMicros > m) {
      fanList[b].rpmMicros = m;
      continue;
    }
    //Serial.print(fanList[b].rpmCount);
    //Serial.print('/');
    //Serial.print(m - fanList[b].rpmMicros);
    //Serial.print('=');
    if (fanList[b].rpmCount == 0) {
      fanList[b].rpm = 0;
    } else {
      fanList[b].rpm = ((float) fanList[b].rpmCount) / (((float)(m - fanList[b].rpmMicros)) / 3.0E+7);
      fanList[b].rpmMicros = m;
      fanList[b].rpmCount = 0ul;
    }
    //Serial.println(fanList[b].rpm);
    if (fanRunSec(b) > 5ul && fanRpm(b) < 1000) {
      beep(1000);
    }
    if (fanList[b].stopSec < rtcUnixtime()) {
      analogWrite(fanList[b].pin, 0);
    }
  }
}

void fanRunAll(unsigned long stopUnixtime) {
  fanRun(0, stopUnixtime);
  fanRun(1, stopUnixtime);
}

void fanRun(byte fan, unsigned long stopUnixtime) {
  unsigned long ut = rtcUnixtime();
  if (fanList[fan].lastWorkDay != fanDayId()) {
    fanList[fan].lastWorkDay = fanDayId();
    fanList[fan].lastWorkDaySec = 0;
  }
  if (fanList[fan].stopSec > ut) {
    // Кулер работает
    fanList[fan].lastWorkDaySec -= fanList[fan].stopSec - ut;
  }
  fanList[fan].lastWorkDaySec += stopUnixtime - ut;

  fanList[fan].stopSec = stopUnixtime;
  if (fan == 0) {
    digitalWrite(fanList[fan].pin, HIGH);
  } else {
    digitalWrite(fanList[fan].pin, HIGH);
  }
}
void fanStopAll() {
  fanStop(0);
  fanStop(1);
}

void fanStop(byte fan) {
  unsigned long ut = rtcUnixtime();
  if (fanList[fan].stopSec > ut) {
    fanList[fan].lastWorkDaySec -= (fanList[fan].stopSec - ut);
    fanList[fan].stopSec = ut;
  }
}

void fanSens1() {
  //Serial.println("fanSens1");
  fanList[0].rpmCount++;
}

void fanSens2() {
  //Serial.println("fanSens2");
  fanList[1].rpmCount++;
}

long fanRpm(byte fan) {
  return fanList[fan].rpm;
}

unsigned long fanLastWorkDay(byte fan) {
  return fanList[fan].lastWorkDay;
}

unsigned long fanLastWorkDaySec(byte fan) {
  return fanList[fan].lastWorkDaySec;
}

// Сколько секунд еще работать
unsigned long fanRunSecMin() {
  return min(fanRunSec(0), fanRunSec(1));
}

// Сколько секунд еще работать
unsigned long fanRunSec(byte fan) {
  unsigned long ut = rtcUnixtime();
  if (ut > fanList[fan].stopSec) {
    return 0ul;
  }
  return fanList[fan].stopSec - ut;
}

bool fanIsRunAll() {
  unsigned long ut = rtcUnixtime();
  return fanList[0].stopSec > ut && fanList[1].stopSec > ut;
}

bool fanIsRun(byte fan) {
  return fanList[fan].stopSec > rtcUnixtime();
}

// Сколько секунд кулер не работал
unsigned long fanTimeOutOfWorkMax() {
  return max(fanTimeOutOfWork(0), fanTimeOutOfWork(1));
}

// Сколько секунд кулер не работал
unsigned long fanTimeOutOfWork(byte b) {
  unsigned long ut = rtcUnixtime();
  if (fanList[b].stopSec > ut) {
    return 0ul;
  }
  return ut - fanList[b].stopSec;
}

// Уникальный идентификатор дня в календаре начиная с 2000 года
unsigned long fanDayId() {
  DateTime now = rtcNow();
  return ((unsigned long) now.year()) * 10000ul  + ((unsigned long) now.month()) * 100ul + ((unsigned long) now.day());
}

