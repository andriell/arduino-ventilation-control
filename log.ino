struct LogStruct {
  float t;
  float h;
  int s;
  int e;
};

LogStruct todayT = {100.0, 100.0, 0, 0};
LogStruct todayHa = {100.0, 100.0, 0, 0};
LogStruct yesterdayT = {100.0, 100.0, 0, 0};
LogStruct yesterdayHa = {100.0, 100.0, 0, 0};

unsigned long logDayId = 0ul;

void logLoop() {
  float t = dhtTOutside();
  float h = dhtHOutside();
  if (isnan(t) || isnan(h)) {
    return;
  }

  if (logDayId != rtcDayId()) {
    yesterdayT = todayT;
    yesterdayHa = todayHa;
    todayT = {100.0, 100.0, 0, 0};
    todayHa = {100.0, 100.0, 0, 0};
    logDayId = rtcDayId();
  }

  if (t < todayT.t) {
    todayT.t = t;
    todayT.h = h;
    todayT.s = rtcHm();
    todayT.e = todayT.s;
  } else if (t == todayT.t) {
    todayT.e = rtcHm();
  }

  float ha = dhtHA(t, h);
  float minHa = dhtHA(todayHa.t, todayHa.h);
  if (ha < minHa) {
    todayHa.t = t;
    todayHa.h = h;
    todayHa.s = rtcHm();
    todayHa.e = todayHa.s;
  } else if (ha == minHa) {
    todayHa.e = rtcHm();
  }
}

void logMenu() {
  logDisplay(true,  true,  todayT,      0);
  logDisplay(false, true,  todayHa,     2);
  logDisplay(true,  false, yesterdayT,  4);
  logDisplay(false, false, yesterdayHa, 6);
  
  if (controlC()) {
    menuOpen(255);
  }
}

void logDisplay(bool isT, bool isToday, struct LogStruct ls, int y) {
  if (isT) {
    oledPrint("T\0",  0, y, 0);
  } else {
    oledPrint("HA\0", 0, y, 0);
  }
  infoDisplayTHHa(y, ls.t, ls.h);

  if (isToday) {
    oledPrint("\341\245\243\0", 0, y + 1, 0);
  } else {
    oledPrint("\242\347\340\0", 0, y + 1, 0);
  }
  oledPrint(strHM(ls.s), 4 * 6, y + 1, 0);
  oledPrint(strHM(ls.e), 13 * 6, y + 1, 0);
}

