// Карта битов
//
// |                 | Температура | Влажность | Температура + Влажность | Расписание |
// | --------------- | ----------- | --------- | ----------------------- | ---------- |
// | Доступно        | 0           | 1         | 2                       | 3          |
// | Кулер включался | 4           | 5         | 6                       | 7          |
//
byte prog1State = 0;
byte prog1MenuLine = 0;
unsigned long prog1MenuLineUnixtime = 0ul;

void prog1Loop() {
  DateTime now = rtcNow();
  int nowMD = (now.month() - 1) * 31 + now.day() - 1;
  int nowHM = now.hour() * 60 + now.minute();

  bitSet(prog1State, 3);
  if (cfgGetDryTimeHM() == nowHM && funTimeOutOfWorkMax() >= cfgGetDryTimeIntervalDay() * 86400ul - 7200ul) {
    // Работа по расписанию
    bitClear(prog1State, 4);
    bitClear(prog1State, 5);
    bitClear(prog1State, 6);
    bitSet(prog1State, 7);
    funRunAll(cfgGetDryTimeRunTimeS() + rtcUnixtime());
  }
  if ((dhtTOutside() == 0.0 && dhtHOutside() == 0.0) || (dhtTCellar() == 0.0 && dhtHCellar() == 0.0)) {
    // Нет показаний с оного из датчиков
    return;
  }
  if (rtcBetweenSerially(nowMD, cfgGetTModStartMD(), cfgGetTModEndMD())) {
    // Работа по температуре
    bitSet(prog1State, 0);
    bitClear(prog1State, 1);
    bitClear(prog1State, 2);
    if (funRunSecMin() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside())) {
      bitSet(prog1State, 4);
      bitClear(prog1State, 5);
      bitClear(prog1State, 6);
      bitClear(prog1State, 7);
      funRunAll(60ul + rtcUnixtime());
    }
  } else if (rtcBetweenSerially(nowMD, cfgGetHModStartMD(), cfgGetHModEndMD())) {
    // Работа по влажности
    bitClear(prog1State, 0);
    bitSet(prog1State, 1);
    bitClear(prog1State, 2);
    if (funRunSecMin() < 10 && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHChange(dhtHOutside(), dhtTOutside(), prog1ResultantT()))) {
      bitClear(prog1State, 4);
      bitSet(prog1State, 5);
      bitClear(prog1State, 6);
      bitClear(prog1State, 7);
      funRunAll(60ul + rtcUnixtime());
    }
  } else {
    // Работа по температуре и по влажности
    bitClear(prog1State, 0);
    bitClear(prog1State, 1);
    bitSet(prog1State, 2);
    if (funRunSecMin() < 10 && prog1NeedRun(dhtTCellar(), cfgGetMinT(), cfgGetMaxT(), dhtTOutside()) && prog1NeedRun(dhtHCellar(), cfgGetMinH(), cfgGetMaxH(), dhtHChange(dhtHOutside(), dhtTOutside(), prog1ResultantT()))) {
      bitClear(prog1State, 4);
      bitClear(prog1State, 5);
      bitSet(prog1State, 6);
      bitClear(prog1State, 7);
      funRunAll(60ul + rtcUnixtime());
    }
  }
}

void prog1Menu() {
  // Строка 0
  oledPrint(rtcStr(), 0, 0, 0);
  oledPrint(rtcWStr(), 19 * 6, 0, 0);

  oledPrint(dhtTStr(dhtTCellar()), 0, 3, 1);
  oledPrint(dhtHStr(dhtHCellar()), 92, 3, 1);

  // Строка 5
  oledPrint(dhtTStr(dhtTCellar()), 0, 5, 0);
  oledPrint(dhtHStr(dhtHCellar()), 6 * 8, 5, 0);
  if (funIsRun(0)) {
    oledInvText(true);
  }
  oledPrint("F1\0", 16 * 6, 5, 0);
  oledInvText(false);
  if (funIsRun(1)) {
    oledInvText(true);
  }
  oledPrint("F2\0", 19 * 6, 5, 0);
  oledInvText(false);

  // Строка 6
  if (prog1MenuLineUnixtime <= rtcUnixtime()) {
    prog1MenuLine++;
    if (prog1MenuLine > 3) {
      prog1MenuLine = 0;
    }
    prog1MenuLineUnixtime = rtcUnixtime() + cfgGetRotationSec();
  }
  oledPrint("           \0", 0, 7, 1);
  if (prog1MenuLine == 0) {
    if (bitRead(prog1State, 0)) {
      if (bitRead(prog1State, 4)) {
        oledInvText(true);
      }
      oledPrint("TE\0", 2 * 6, 6, 0);
      oledInvText(false);
    }
    if (bitRead(prog1State, 1)) {
      if (bitRead(prog1State, 5)) {
        oledInvText(true);
      }
      oledPrint("HU\0", 6 * 6, 6, 0);
      oledInvText(false);
    }
    if (bitRead(prog1State, 2)) {
      if (bitRead(prog1State, 6)) {
        oledInvText(true);
      }
      oledPrint("TE+HU\0", 10 * 6, 6, 0);
      oledInvText(false);
    }
    if (bitRead(prog1State, 3)) {
      if (bitRead(prog1State, 7)) {
        oledInvText(true);
      }
      oledPrint("SC\0", 17 * 6, 6, 0);
      oledInvText(false);
    }
  } else if (prog1MenuLine == 1) {
    oledPrint(dhtTStr(dhtTOutside()), 6, 6, 0);
    prog1DisplyProcess();
    oledPrint(dhtTStr(dhtTCellar()), 6 * 13, 6, 0);
  } else if (prog1MenuLine == 2) {
    oledPrint(dhtHStr(dhtHOutside()), 6 * 3, 6, 0);
    prog1DisplyProcess();
    oledPrint(dhtHStr(dhtHCellar()), 6 * 15, 6, 0);
  } else if (prog1MenuLine == 3) {
    oledPrint(dhtHAStr(dhtTOutside(), dhtHOutside()), 6, 6, 0);
    prog1DisplyProcess();
    oledPrint(dhtHAStr(dhtTCellar(), dhtHCellar()), 6 * 14, 6, 0);
  }

  // Строка 7
  if (prog1MenuLine == 0) {
    infoDisplayFanRumInfo(0, 7);
  } else if (prog1MenuLine == 1) {
    infoDisplayFanWorkInfo(0, 7);
  } else if (prog1MenuLine == 2) {
    infoDisplayFanRumInfo(1, 7);
  } else if (prog1MenuLine == 3) {
    infoDisplayFanWorkInfo(1, 7);
  }

  // Контроль
  if (controlE() || controlP()) {
    runOpenMenu();
  }
  if (controlM()) {
    funStopAll();
  }
  if (controlC()) {
    menuOpen(255);
  }
  
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
// Какая температура будет в погребе в результате проветривания
float prog1ResultantT() {
  if (dhtTCellar() < dhtTOutside()) {
    return cfgGetMaxT();
  }
  if (dhtTCellar() > dhtTOutside()) {
    return cfgGetMinT();
  }
  return dhtTOutside();
}

void prog1DisplyProcess() {
  if (funIsRunAll()) {
    oledPrint(">>>\0", 9 * 6, 6, 0);
  } else {
    oledPrint("|||\0", 9 * 6, 6, 0);
  }
}

