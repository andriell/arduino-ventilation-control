// Карта битов
//
// |                 | Температура | Влажность | Температура + Влажность | Расписание |
// | --------------- | ----------- | --------- | ----------------------- | ---------- |
// | Доступно конфиг | 0           | 1         | 2                       | 3          |
// | Доступно сейчас | 4           | 5         | 6*                      | 7*         |
//
int modState = 0;

void modLoop() {
  DateTime now = rtcNow();
  int nowMD = rtcMd();
  int nowHM = rtcHm();
  modState = cfgMod();
  //Serial.println();
  //Serial.print(modState);
  //Serial.print(' ');
  //Serial.print(modState, BIN);
  if (bitRead(modState, 3)) {
    // Работа по расписанию
    bitSet(modState, 7);
    if (cfgGetDryTimeHM() == nowHM && fanTimeOutOfWork() >= cfgGetDryTimeIntervalDay() * 86400ul - 7200ul) {
      fanRun(cfgGetDryTimeRunTimeS() + rtcUnixtime(), 0);
    }
  }
  if (isnan(dhtTOutside()) || isnan(dhtHOutside()) || isnan(dhtTCellar()) || isnan(dhtHCellar())) {
    // Нет показаний с оного из датчиков
    return;
  }
  if (bitRead(modState, 0) && rtcBetweenSerially(nowMD, cfgGetTModStartMD(), cfgGetTModEndMD())) {
    // Работа по температуре
    bitSet(modState, 4);
    if (fanSecToStop() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside())) {
      fanRun(60ul + rtcUnixtime(), 0);
    }
  }
  if (bitRead(modState, 1) && rtcBetweenSerially(nowMD, cfgGetHModStartMD(), cfgGetHModEndMD())) {
    // Работа по влажности
    bitSet(modState, 5);
    if (fanSecToStop() < 10 && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHChange(dhtHOutside(), dhtTOutside(), prog1ResultantT()))) {
      fanRun(60ul + rtcUnixtime(), 0);
    }
  }
  if (bitRead(modState, 2)) {
    // Работа по температуре и по влажности
    bitSet(modState, 6);
    if (fanSecToStop() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside()) && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHChange(dhtHOutside(), dhtTOutside(), prog1ResultantT()))) {
      fanRun(60ul + rtcUnixtime(), 0);
    }
  }
  //Serial.print(' ');
  //Serial.print(modState, BIN);
}

void modDisplay(int y) {
  if (bitRead(modState, 0)) {
    if (bitRead(modState, 4)) {
      oledInvText(true);
    }
    oledPrint("TE\0", 2 * 6, y, 0);
    oledInvText(false);
  }
  if (bitRead(modState, 1)) {
    if (bitRead(modState, 5)) {
      oledInvText(true);
    }
    oledPrint("HU\0", 6 * 6, y, 0);
    oledInvText(false);
  }
  if (bitRead(modState, 2)) {
    if (bitRead(modState, 6)) {
      oledInvText(true);
    }
    oledPrint("TE+HU\0", 10 * 6, y, 0);
    oledInvText(false);
  }
  if (bitRead(modState, 3)) {
    if (bitRead(modState, 7)) {
      oledInvText(true);
    }
    oledPrint("SC\0", 17 * 6, y, 0);
    oledInvText(false);
  }
}

