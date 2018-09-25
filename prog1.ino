
void prog1Loop() {
  DateTime now = rtcNow();
  int nowMD = (now.month() - 1) * 31 + now.day() - 1;
  int nowHM = now.hour() * 60 + now.minute();
  int nowMS = now.minute() * 60 + now.second();
  if (prog1BetweenSerially(nowMD, cfgGetTModStartMD(), cfgGetTModEndMD())) {
    // Работа по температуре
    if (funRunSecMin() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside())) {
      funRunAll(60);
    }
  } else if (prog1BetweenSerially(nowMD, cfgGetHModStartMD(), cfgGetHModEndMD())) {
    // Работа по влажности
    if (funRunSecMin() < 10 && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHOutside())) {
      funRunAll(60);
    }
  } else {
    // Работа по температуре и по влажности
    if (funRunSecMin() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside()) && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHOutside())) {
      funRunAll(60);
    }
  }
  if (cfgGetDryTimeHM() == nowHM && funTimeOutOfWorkMax() >= cfgGetDryTimeIntervalDay() * 86400ul - 7200ul) {
    // Работа по расписанию
    funRunAll(cfgGetDryTimeRunTimeS() + rtcUnixtime());
  }
}
// Проверяет располагается ли циклическая величина v в интервале от vMin до vMax включая концы
bool prog1BetweenSerially(int v, int vMin, int vMax) {
  if (vMin <= vMax) {
    return vMin <= v && v <= vMax;
  }
  return vMin <= v || v <= vMax;
}
// Нужно ли включать вентиляцию
bool prog1NeedRun(float v, float vMin, float vMax, float vOutside) {
  if (vMin <= v && v <= vMax) {
    return true;
  }
  if (v < vMin && vOutside > vMin) {
    return true;
  }
  if (v > vMax && vOutside < vMax) {
    return true;
  }
  return false;
}

