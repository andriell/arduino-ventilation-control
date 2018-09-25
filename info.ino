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
    oledPrint("F\0", 0, 5 + b, 0);
    oledPrintByte(b + 1, 6, 5 + b, 0);
    oledPrint(funSecStr(b), 6 * 3, 5 + b, 0);
    oledPrint(funRpmStr(b), 6 * 13, 5 + b, 0);
  }
  
  oledPrintByte(infoFan + 1, 0, 7, 0);
  oledPrint(funLastWorkDayStr(infoFan), 6 * 2, 7, 0);
  oledPrint(funLastWorkDaySecStr(infoFan), 6 * 13, 7, 0);

  if (infoFan == 0) {
    infoFan = 1;
  } else {
    infoFan = 0;
  }

  // Контроль
  if (controlP()) {
    funRun(0, 60);
    funRun(1, 60);
  }
  if (controlM()) {
    funStop(0);
    funStop(1);
  }
  if (controlC()) {
    menuOpen(255);
  }
}

