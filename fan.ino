#define FAN_IN_PIN_HZ 2
#define FAN_OUT_PIN_HZ 3
#define FAN_IN_PIN 5
#define FAN_OUT_PIN 6
#define ONE_SECOND 1000000ul

unsigned long fanStopSec;
byte fanMode = 0; // 0 - попеременно, 1 - in, 2 - out
unsigned long fanRpmMicros;
unsigned long fanRpmCount;
unsigned long fanRpm;
unsigned long fanLastWorkDay;
unsigned long fanLastWorkDaySec;

void fanSetup() {
  pinMode(FAN_IN_PIN, OUTPUT);
  pinMode(FAN_OUT_PIN, OUTPUT);
  analogWrite(FAN_IN_PIN, 0);
  analogWrite(FAN_OUT_PIN, 0);
  pinMode(FAN_IN_PIN_HZ, INPUT_PULLUP);
  pinMode(FAN_OUT_PIN_HZ, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FAN_IN_PIN_HZ), fanSens, FALLING);
  attachInterrupt(digitalPinToInterrupt(FAN_OUT_PIN_HZ), fanSens, FALLING);
}

void fanLoop() {
  unsigned long m = micros();
  if (fanRpmMicros > m) {
    fanRpmMicros = m;
    return;
  }
  //Serial.print(fanRpmCount);
  //Serial.print('/');
  //Serial.print(m - fanRpmMicros);
  //Serial.print('=');
  if (fanRpmCount == 0) {
    fanRpm = 0;
  } else {
    fanRpm = ((float) fanRpmCount) / (((float)(m - fanRpmMicros)) / 3.0E+7);
    fanRpmMicros = m;
    fanRpmCount = 0ul;
  }
  //Serial.println(fanRpm);
  
  if (!fanIsRun()) {
    analogWrite(FAN_IN_PIN, LOW);
    analogWrite(FAN_OUT_PIN, LOW);
    return;
  }
  if (fanIsIn()) {
    digitalWrite(FAN_IN_PIN, HIGH);
  } else {
    digitalWrite(FAN_IN_PIN, HIGH);
  }
  if (fanRunSec() > 5ul && fanRpm < 1000) {
    beep(1000);
  }
}

bool fanIsIn() {
  if (fanMode == 1) {
    return true;
  }
  if (fanMode == 0) {
    unsigned long ut = rtcUnixtime();
    if (fanStopSec < ut) {
      return true;
    }
    return (((int) (fanStopSec - ut)) / cfgFanRotationTime()) % 2 == 0;
  }
  return false;
}


void fanRun(unsigned long stopUnixtime, byte mode) {
  fanMode = mode;
  unsigned long ut = rtcUnixtime();
  if (fanLastWorkDay != fanDayId()) {
    fanLastWorkDay = fanDayId();
    fanLastWorkDaySec = 0;
  }
  if (fanIsRun()) {
    // Кулер работает
    fanLastWorkDaySec -= fanStopSec - ut;
  }
  fanLastWorkDaySec += stopUnixtime - ut;

  fanStopSec = stopUnixtime;
}

void fanStop() {
  unsigned long ut = rtcUnixtime();
  if (fanIsRun()) {
    fanLastWorkDaySec -= (fanStopSec - ut);
    fanStopSec = ut;
  }
}

void fanSens() {
  //Serial.println("fanSens");
  fanRpmCount++;
}

long fanGetRpm() {
  return fanRpm;
}

unsigned long fanGetLastWorkDay() {
  return fanLastWorkDay;
}

unsigned long fanGetLastWorkDaySec() {
  return fanLastWorkDaySec;
}

// Сколько секунд еще работать
unsigned long fanRunSec() {
  if (fanIsRun()) {
    return fanStopSec - rtcUnixtime();
  }
  return 0ul;
}

// Сколько секунд кулер не работал
unsigned long fanTimeOutOfWork() {
  if (fanIsRun()) {
    return 0ul;
  }
  return rtcUnixtime() - fanStopSec;
}


bool fanIsRun() {
  return fanStopSec > rtcUnixtime();
}

// Уникальный идентификатор дня в календаре начиная с 2000 года
unsigned long fanDayId() {
  DateTime now = rtcNow();
  return ((unsigned long) now.year()) * 10000ul  + ((unsigned long) now.month()) * 100ul + ((unsigned long) now.day());
}

