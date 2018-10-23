#define FAN_IN_PIN_HZ 2
#define FAN_OUT_PIN_HZ 3
#define FAN_IN_PIN 5
#define FAN_OUT_PIN 6
#define ONE_SECOND 1000000ul

unsigned long fanStopSec;
unsigned long fanStartSec;
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
  byte runFanId = fanRunId();
  if (runFanId == 0) {
    analogWrite(FAN_IN_PIN, LOW);
    analogWrite(FAN_OUT_PIN, LOW);
    return;
  }
  if (runFanId == 1 xor cfgFanInOut()) {
    digitalWrite(FAN_IN_PIN, HIGH);
    analogWrite(FAN_OUT_PIN, LOW);
  } else {
    analogWrite(FAN_IN_PIN, LOW);
    digitalWrite(FAN_OUT_PIN, HIGH);
  }
  if (fanRunDirectionSec() > 5 && fanRpm < 1000) {
    beep(1000);
  }
}
// Номер включенного вентилятора. 0 - все выключено, 1 - in, 2 - out
byte fanRunId() {
  if (!fanIsRun()) {
    return 0;
  }
  if (fanMode == 0) {
    // Serial.print(" ");
    // Serial.print((int) fanRunSec());
    // Serial.print(" ");
    // Serial.print(cfgFanRotationTime());
    // Serial.print(" ");
    // Serial.print(((int) fanRunSec()) / cfgFanRotationTime());
    // Serial.print(" ");
    // Serial.print((((int) fanRunSec()) / cfgFanRotationTime()) % 2);
    // Serial.println();
    return (((int) fanRunSec()) / cfgFanRotationTime()) % 2 == 0 ? 1 : 2;
  } else if (fanMode == 1) {
    return 1;
  }
  return 2;
}


void fanRun(unsigned long stopUnixtime, byte mode) {
  fanMode = mode;
  unsigned long ut = rtcUnixtime();
  if (fanLastWorkDay != rtcDayId()) {
    fanLastWorkDay = rtcDayId();
    fanLastWorkDaySec = 0;
  }
  if (fanIsRun()) {
    // Кулер работает
    fanLastWorkDaySec -= fanStopSec - ut;
  } else {
    fanStartSec = ut;
  }
  fanLastWorkDaySec += stopUnixtime - ut;

  fanStopSec = stopUnixtime;
}

void fanStop() {
  if (fanIsRun()) {
    unsigned long ut = rtcUnixtime();
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
unsigned long fanSecToStop() {
  if (fanIsRun()) {
    return fanStopSec - rtcUnixtime();
  }
  return 0ul;
}
// Сколько секунд уже работает
unsigned long fanRunSec() {
  if (!fanIsRun()) {
    return 0ul;
  }
  unsigned long ut = rtcUnixtime();
  if (ut < fanStartSec) {
    return 0ul;
  }
  return ut - fanStartSec;
}

// Сколько секунд уже работает в этом направлении
int fanRunDirectionSec() {
  if (fanMode != 0) {
    return (int) fanRunSec();
  }
  int runSec = (int) fanRunSec();
  if (runSec <= 0) {
    return 0;
  }
  return runSec - (runSec / cfgFanRotationTime()) * cfgFanRotationTime();
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

