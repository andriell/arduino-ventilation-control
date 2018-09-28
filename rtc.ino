RTC_DS1307 rtcVar;
byte rtcInsertI = 0;
unsigned long rtcLastNowMicros = 0;
DateTime rtcLastNow;

void rtcSetup() {
  rtcVar.begin();
  rtcLastNow = rtcVar.now();
}

void rtcLoop() {
  rtcLastNow = rtcVar.now();
}

void rtcMenu() {
  menuTitile();
  DateTime now = rtcNow();

  if (rtcInsertI == 1) {
    oledInvText(true);
  }
  oledPrintInt(now.year(), 0, 2, 1);
  oledInvText(false);
  oledPrint("-\0", 48, 2, 1);

  if (rtcInsertI == 2) {
    oledInvText(true);
  }
  oledPrint(rtc02d(now.month()), 60, 2, 1);
  oledInvText(false);
  oledPrint("-\0", 84, 2, 1);

  if (rtcInsertI == 3) {
    oledInvText(true);
  }
  oledPrint(rtc02d(now.day()), 96, 2, 1);
  oledInvText(false);

  if (rtcInsertI == 4) {
    oledInvText(true);
  }
  oledPrint(rtc02d(now.hour()), 0, 5, 1);
  oledInvText(false);
  oledPrint(":\0", 24, 4, 1);

  if (rtcInsertI == 5) {
    oledInvText(true);
  }
  oledPrint(rtc02d(now.minute()), 36, 5, 1);
  oledInvText(false);
  oledPrint(":\0", 60, 4, 1);

  if (rtcInsertI == 6) {
    oledInvText(true);
  }
  oledPrint(rtc02d(now.second()), 72, 5, 1);
  oledInvText(false);

  if (rtcInsertI == 7) {
    oledInvText(true);
  }
  oledPrint(rtcWStr(), 102, 5, 0);
  oledInvText(false);

  // Контроль
  int rtcInsertVal = 0;
  if (controlP()) {
    if (rtcInsertI > 0 && rtcInsertI <= 7) {
      rtcInsertVal = rtcGet(rtcInsertI);
      rtcInsertVal++;
      rtcSet(rtcInsertI, rtcInsertVal);
    }
  }
  if (controlM()) {
    if (rtcInsertI > 0 && rtcInsertI <= 7) {
      rtcInsertVal = rtcGet(rtcInsertI);
      rtcInsertVal--;
      rtcSet(rtcInsertI, rtcInsertVal);
    }
  }
  if (controlE()) {
    rtcInsertI++;
    if (rtcInsertI > 7) {
      rtcInsertI = 0;
    }
  }
  if (controlC()) {
    if (rtcInsertI > 0) {
      rtcInsertI = 0;
    } else {
      menuOpen(255);
    }
  }
}

void rtcSet(byte i, int val) {
  // settime(секунды [, минуты [, часы [, день [, месяц [, год [, день недели]]]]]])
  DateTime now = rtcNow();
  if (i == 6) {
    // Секунды
    if (val < 0) {
      val = 59;
    } else if (val > 59) {
      val = 0;
    }
    rtcVar.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), val));
  } else if (i == 5) {
    // Минуты
    if (val < 0) {
      val = 59;
    } else if (val > 59) {
      val = 0;
    }
    rtcVar.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), val, now.second()));
  } else if (i == 4) {
    // Часы
    if (val < 0) {
      val = 23;
    } else if (val > 23) {
      val = 0;
    }
    rtcVar.adjust(DateTime(now.year(), now.month(), now.day(), val, now.minute(), now.second()));
  } else if (i == 3) {
    // Дни
    if (val < 1) {
      val = 31;
    } else if (val > 31) {
      val = 1;
    }
    rtcVar.adjust(DateTime(now.year(), now.month(), val, now.hour(), now.minute(), now.second()));
  } else if (i == 2) {
    // Месяцы
    if (val < 1) {
      val = 12;
    } else if (val > 12) {
      val = 1;
    }
    rtcVar.adjust(DateTime(now.year(), val, now.day(), now.hour(), now.minute(), now.second()));
  } else if (i == 1) {
    // Годы
    // год указывается без учёта века, в формате 0-99
    val = constrain(val, 2018, 2099);
    rtcVar.adjust(DateTime(val, now.month(), now.day(), now.hour(), now.minute(), now.second()));
  }
}

int rtcGet(byte i) {
  DateTime now = rtcNow();
  if (i == 6) {
    return now.second();
  } else if (i == 5) {
    return now.minute();
  } else if (i == 4) {
    return now.hour();
  } else if (i == 3) {
    return now.day();
  } else if (i == 2) {
    return now.month();
  } else if (i == 1) {
    return now.year();
  } else if (i == 7) {
    return now.dayOfTheWeek();
  }
  return -1;
}

// День недели 2 символа
char* rtcWStr() {
  // 0-воскресенье до 6-суббота
  DateTime now = rtcNow();
  int w = now.dayOfTheWeek();
  if (w == 0) {
    return "\202\341\0";
  } else if (w == 1) {
    return "\217\255\0";
  } else if (w == 2) {
    return "\202\342\0";
  } else if (w == 3) {
    return "\221\340\0";
  } else if (w == 4) {
    return "\227\342\0";
  } else if (w == 5) {
    return "\217\342\0";
  } else if (w == 6) {
    return "\221\241\0";
  }
  return "\0";
}

// 2 символа
char* rtc02d(int i) {
  sprintf(char22, "%02d\0", i);
  char22[2] = '\0';
  return char22;
}

DateTime rtcNow() {
  return rtcLastNow;
}

unsigned long rtcUnixtime() {
  return rtcLastNow.unixtime();
}

// Полная дата. 19 символов
char* rtcStr() {
  DateTime now = rtcNow();
  sprintf(char22, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  char22[19] = '\0';
  return char22;
}

// Проверяет располагается ли циклическая величина v в интервале от vMin до vMax включая концы
bool rtcBetweenSerially(int v, int vMin, int vMax) {
  if (vMin <= vMax) {
    return vMin <= v && v <= vMax;
  }
  return vMin <= v || v <= vMax;
}

