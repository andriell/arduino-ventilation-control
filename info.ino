
void infoMenu() {
  oledPrint(strDate(), 0, 0, 0);
  oledPrint(rtcWStr(), 19 * 6, 0, 0);

  oledPrint("In\0", 0, 1, 0);
  infoDisplayTHHa(1, dhtTCellar(), dhtHCellar());
  
  oledPrint("Out\0", 0, 2, 0);
  infoDisplayTHHa(2, dhtTOutside(), dhtHOutside());
  
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

void infoDisplayTHHa(byte y, float t, float h) {
  oledPrint(strT(t), 6 * 3, y, 0);
  oledPrint(strH(h), 6 * 11, y, 0);
  oledPrint(strHA(t, h), 6 * 15, y, 0);
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

