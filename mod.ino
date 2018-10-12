// Карта битов
//
// |                 | Температура | Влажность | Температура + Влажность | Расписание |
// | --------------- | ----------- | --------- | ----------------------- | ---------- |
// | Доступно конфиг | 0           | 1         | 2                       | 3          |
// | Доступно сейчас | 4           | 5         | 6                       | 7          |
//
int modState = 0;

void modLoop() {
  DateTime now = rtcNow();
  int nowMD = (now.month() - 1) * 31 + now.day() - 1;
  int nowHM = now.hour() * 60 + now.minute();
  modState = cfgMod();
  //Serial.println();
  //Serial.print(modState);
  //Serial.print(' ');
  //Serial.print(modState, BIN);
  if (bitRead(modState, 3) && cfgGetDryTimeHM() == nowHM && fanTimeOutOfWorkMax() >= cfgGetDryTimeIntervalDay() * 86400ul - 7200ul) {
    // Работа по расписанию
    bitSet(modState, 7);
    fanRunAll(cfgGetDryTimeRunTimeS() + rtcUnixtime());
  }
  if (isnan(dhtTOutside()) || isnan(dhtHOutside()) || isnan(dhtTCellar()) || isnan(dhtHCellar())) {
    // Нет показаний с оного из датчиков
    return;
  }
  if (bitRead(modState, 0) && rtcBetweenSerially(nowMD, cfgGetTModStartMD(), cfgGetTModEndMD())) {
    // Работа по температуре
    bitSet(modState, 4);
    if (fanRunSecMin() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside())) {
      fanRunAll(60ul + rtcUnixtime());
    }
  } else if (bitRead(modState, 1) && rtcBetweenSerially(nowMD, cfgGetHModStartMD(), cfgGetHModEndMD())) {
    // Работа по влажности
    bitSet(modState, 5);
    if (fanRunSecMin() < 10 && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHChange(dhtHOutside(), dhtTOutside(), prog1ResultantT()))) {
      fanRunAll(60ul + rtcUnixtime());
    }
  } else if (bitRead(modState, 2)) {
    // Работа по температуре и по влажности
    bitSet(modState, 6);
    if (fanRunSecMin() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside()) && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHChange(dhtHOutside(), dhtTOutside(), prog1ResultantT()))) {
      fanRunAll(60ul + rtcUnixtime());
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

