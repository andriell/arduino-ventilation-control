
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
  infoDisplayFanRumInfo(4);
  infoDisplayFanWorkInfo(5);

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

void infoDisplayFanRumInfo(byte y) {
  oledPrint(strFan(), 0, y, 0);
  oledPrint(strHMS(fanSecToStop()), 6 * 3, y, 0);
  oledPrint(strFanRpm(), 6 * 13, y, 0);
}

void infoDisplayFanWorkInfo(byte y) {
  oledPrint(strFanLastWorkDay(), 0, y, 0);
  oledPrint(strHMS(fanGetLastWorkDaySec()), 6 * 13, y, 0);
}

