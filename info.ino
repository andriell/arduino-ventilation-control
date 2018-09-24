byte infoFan = 0;

void infoMenu() {
  oledPrint(rtcStr(), 0, 0, 0);
  oledPrint(rtcWStr(), 0, 19 * 6, 0);
  float t, h;
  for (byte b = 0; b < DHT_SIZE; b++) {
    t = dhtT(b);
    h = dhtH(b);
    oledPrintByte(b, b + 1, 0, 0);
    oledPrint(dhtTStr(t), b + 1, 6 * 2, 0);
    oledPrint(dhtHStr(t), b + 1, 6 * 10, 0);
    oledPrint(dhtHAStr(t, h), b + 1, 6 * 14, 0);
  }
  for (byte b = 0; b < 2; b++) {
    oledPrint("F\0", 5 + b, 0, 0);
    oledPrintByte(b + 1, 5 + b, 6, 0);
    oledPrint(funSecStr(b), 5 + b, 6 * 3, 0);
    oledPrint(funRpmStr(b), 5 + b, 6 * 13, 0);
  }
  
  oledPrintByte(infoFan + 1, 7, 0, 0);
  oledPrint(funLastWorkDayStr(infoFan), 7, 6 * 2, 0);
  oledPrint(funLastWorkDaySecStr(infoFan), 7, 6 * 13, 0);

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

