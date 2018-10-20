byte prog1MenuLine = 0;
unsigned long prog1MenuLineUnixtime = 0ul;

void prog1Menu() {
  // Строка 0
  oledPrint(strDate(), 0, 0, 0);
  oledPrint(rtcWStr(), 19 * 6, 0, 0);

  oledPrint(strT(dhtTCellar()), 0, 3, 1);
  oledPrint(strH(dhtHCellar()), 92, 3, 1);

  // Строка 5
  oledPrint(strT(dhtTOutside()), 0, 5, 0);
  oledPrint(strH(dhtHOutside()), 6 * 8, 5, 0);
  if (fanIsRun()) {
    oledInvText(true);
  }
  oledPrint(strFan(), 19 * 6, 5, 0);
  oledInvText(false);

  // Строка 6
  if (prog1MenuLineUnixtime <= rtcUnixtime()) {
    prog1MenuLine++;
    if (prog1MenuLine > 3) {
      prog1MenuLine = 0;
    }
    prog1MenuLineUnixtime = rtcUnixtime() + cfgGetRotationSec();
    oledPrint("           \0", 0, 7, 1);
  }
  if (prog1MenuLine == 0) {
    modDisplay(6);
  } else if (prog1MenuLine == 1) {
    oledPrint(strT(dhtTOutside()), 6, 6, 0);
    prog1DisplyProcess();
    oledPrint(strT(dhtTCellar()), 6 * 13, 6, 0);
  } else if (prog1MenuLine == 2) {
    oledPrint(strH(dhtHOutside()), 6 * 3, 6, 0);
    prog1DisplyProcess();
    oledPrint(strH(dhtHCellar()), 6 * 15, 6, 0);
  } else if (prog1MenuLine == 3) {
    oledPrint(strHA(dhtTOutside(), dhtHOutside()), 6, 6, 0);
    prog1DisplyProcess();
    oledPrint(strHA(dhtTCellar(), dhtHCellar()), 6 * 14, 6, 0);
  }

  // Строка 7
  if (prog1MenuLine == 0) {
    infoDisplayFanRumInfo(7);
  } else if (prog1MenuLine == 1) {
    infoDisplayFanWorkInfo(7);
  } else if (prog1MenuLine == 2) {
    infoDisplayFanRumInfo(7);
  } else if (prog1MenuLine == 3) {
    infoDisplayFanWorkInfo(7);
  }

  // Контроль
  if (controlE() || controlP()) {
    runOpenMenu();
  }
  if (controlM()) {
    fanStop();
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
  if (!fanIsRun()) {
    oledPrint("|||\0", 9 * 6, 6, 0);
  } else if (fanIsIn()) {
    oledPrint(">>>\0", 9 * 6, 6, 0);
  } else {
    oledPrint("<<<\0", 9 * 6, 6, 0);
  }
}

