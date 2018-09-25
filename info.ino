byte infoFan = 0;

void infoMenu() {
  oledPrint(rtcStr(), 0, 0, 0);
  oledPrint(rtcWStr(), 19 * 6, 0, 0);
  float t, h;
  for (byte b = 0; b < DHT_SIZE; b++) {
    t = dhtT(b);
    h = dhtH(b);
    oledPrintByte(b, 0, b + 1, 0);
    oledPrint(dhtTStr(t), 6 * 3, b + 1, 0);
    oledPrint(dhtHStr(t), 6 * 11, b + 1, 0);
    oledPrint(dhtHAStr(t, h), 6 * 15, b + 1, 0);
  }
  for (byte b = 0; b < 2; b++) {
    infoDisplayFanRumInfo(b, 5 + b);
  }

  infoDisplayFanWorkInfo(infoFan, 7);

  if (infoFan == 0) {
    infoFan = 1;
  } else {
    infoFan = 0;
  }

  // Контроль
  if (controlP()) {
    funRunAll(60);
  }
  if (controlM()) {
    funStopAll();
  }
  if (controlC()) {
    menuOpen(255);
  }
}

void infoDisplayFanRumInfo(byte fan, byte y) {
  oledPrint("F\0", 0, y, 0);
  oledPrintByte(fan + 1, 6, y, 0);
  oledPrint(funSecStr(fan), 6 * 3, y, 0);
  oledPrint(funRpmStr(fan), 6 * 13, y, 0);
}

void infoDisplayFanWorkInfo(byte fan, byte y) {
  oledPrintByte(fan + 1, 0, y, 0);
  oledPrint(funLastWorkDayStr(fan), 6 * 2, y, 0);
  oledPrint(funLastWorkDaySecStr(fan), 6 * 13, y, 0);
}

