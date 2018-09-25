int prog1TModStartMD = 0;
int prog1TModEndMD = 0;
int prog1DryTimeHM = 0;
int prog1DryTimeIntervalDay = 0;
int prog1DryTimeRunTimeMS = 0;
unsigned long prog1NextUpate = 0;

void prog1Loop() {
  DateTime now = rtcNow();
  if (prog1NextUpate < now.unixtime()) {
    prog1TModStartMD = cfgGetTModStartMD();
    prog1TModEndMD = cfgGetTModEndMD();
    prog1DryTimeHM = cfgGetDryTimeHM();
    prog1DryTimeIntervalDay = cfgGetDryTimeIntervalDay();
    prog1DryTimeRunTimeMS = cfgGetDryTimeRunTimeMS();
    prog1NextUpate = now.unixtime() + 60ul;
  }
  
  int nowMD = (now.month() - 1) * 31 + now.day() - 1;
  int nowHM = now.hour() * 60 + now.minute();
  int nowMS = now.minute() * 60 + now.second();
  if (prog1Between(nowMD, prog1TModStartMD, prog1TModEndMD)) {
    // Работа по температуре
  } else {
    // Работа по влажности
  }
  
}
// Проверяет располагается ли циклическая величина v в интервале от s до e включая концы
bool prog1Between(int v, int s, int e) {
  if (s <= e) {
    return s <= v && s <= e;
  }
  return s <= v || s <= e;
}

