
void infoMenu() {
  oledPrint(strDate(), 0, 0, 0);
  oledPrint(rtcWStr(), 19 * 6, 0, 0);
  float t, h;
  for (byte b = 0; b < DHT_SIZE; b++) {
    t = dhtT(b);
    h = dhtH(b);
    oledPrintByte(b, 0, b + 1, 0);
    oledPrint(strT(t), 6 * 3, b + 1, 0);
    oledPrint(strH(t), 6 * 11, b + 1, 0);
    oledPrint(strHA(t, h), 6 * 15, b + 1, 0);
  }
  modDisplay(3);
  for (byte b = 0; b < 2; b++) {
    infoDisplayFanRumInfo(b, 4 + b);
    infoDisplayFanWorkInfo(b, 6 + b);
  }

  // Контроль
  if (controlE() || controlP()) {
    runOpenMenu();
  }
  if (controlM()) {
    fanStopAll();
  }
  if (controlC()) {
    menuOpen(255);
  }
}

void infoDisplayFanRumInfo(byte fan, byte y) {
  oledPrint("F\0", 0, y, 0);
  oledPrintByte(fan + 1, 6, y, 0);
  oledPrint(strHMS(fanRunSec(fan)), 6 * 3, y, 0);
  oledPrint(strFanRpm(fan), 6 * 13, y, 0);
}

void infoDisplayFanWorkInfo(byte fan, byte y) {
  oledPrintByte(fan + 1, 0, y, 0);
  oledPrint(strFanLastWorkDay(fan), 6 * 2, y, 0);
  oledPrint(strHMS(fanLastWorkDaySec(fan)), 6 * 13, y, 0);
}

