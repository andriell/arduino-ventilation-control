iarduino_RTC rtcVar(RTC_DS1307);
byte rtcInsertI = 0;

void rtcSetup() {
  rtcVar.begin();
}

void rtcMenu() {
  menuTitile();

  if (rtcInsertI == 1) {
    oledInvText(true);
  }
  oledPrint(rtcVar.gettime("Y"), 0, 2, 1);
  oledInvText(false);
  oledPrint("-", 48, 2, 1);
  
  if (rtcInsertI == 2) {
    oledInvText(true);
  }
  oledPrint(rtcVar.gettime("m"), 60, 2, 1);
  oledInvText(false);
  oledPrint("-", 84, 2, 1);

  if (rtcInsertI == 3) {
    oledInvText(true);
  }
  oledPrint(rtcVar.gettime("d"), 96, 2, 1);
  oledInvText(false);

  if (rtcInsertI == 4) {
    oledInvText(true);
  }
  oledPrint(rtcVar.gettime("H"), 12, 4, 1);
  oledInvText(false);
  oledPrint(":", 36, 4, 1);

  if (rtcInsertI == 5) {
    oledInvText(true);
  }
  oledPrint(rtcVar.gettime("i"), 48, 4, 1);
  oledInvText(false);
  oledPrint(":", 72, 4, 1);

  if (rtcInsertI == 6) {
    oledInvText(true);
  }
  oledPrint(rtcVar.gettime("s"), 84, 4, 1);
  oledInvText(false);

  if (rtcInsertI == 7) {
    oledInvText(true);
  }
  oledPrint(rtcW(), 48, 6, 1);
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
  if (i == 6) {
    // Секунды
    if (val < 0) {
      val = 59;
    } else if (val > 59) {
      val = 0;
    }
    rtcVar.settime(val, -1, -1, -1, -1, -1, -1);
  } else if (i == 5) {
    // Минуты
    if (val < 0) {
      val = 59;
    } else if (val > 59) {
      val = 0;
    }
    rtcVar.settime(-1, val, -1, -1, -1, -1, -1);
  } else if (i == 4) {
    // Часы
    if (val < 0) {
      val = 23;
    } else if (val > 23) {
      val = 0;
    }
    rtcVar.settime(-1, -1, val, -1, -1, -1, -1);
  } else if (i == 3) {
    // Дни
    if (val < 1) {
      val = 31;
    } else if (val > 31) {
      val = 1;
    }
    rtcVar.settime(-1, -1, -1, val, -1, -1, -1);
  } else if (i == 2) {
    // Месяцы
    if (val < 1) {
      val = 12;
    } else if (val > 12) {
      val = 1;
    }
    rtcVar.settime(-1, -1, -1, -1, val, -1, -1);
  } else if (i == 1) {
    // Годы
    // год указывается без учёта века, в формате 0-99
    val = constrain(val, 18, 99);
    rtcVar.settime(-1, -1, -1, -1, -1, val, -1);
  } else if (i == 7) {
    // День недели
    if (val < 0) {
      val = 6;
    } else if (val > 6) {
      val = 0;
    }
    rtcVar.settime(-1, -1, -1, -1, -1, -1, val);
  }
}

int rtcGet(byte i) {
  String val = "0";
  if (i == 6) {
    val = rtcVar.gettime("s");
  } else if (i == 5) {
    val = rtcVar.gettime("i");
  } else if (i == 4) {
    val = rtcVar.gettime("H");
  } else if (i == 3) {
    val = rtcVar.gettime("d");
  } else if (i == 2) {
    val = rtcVar.gettime("m");
  } else if (i == 1) {
    val = rtcVar.gettime("Y");
    val = val.substring(2);
  } else if (i == 7) {
    val = rtcVar.gettime("w");
  }
  return val.toInt();
}

String rtcW() {
  // 0-воскресенье до 6-суббота
  String w = rtcVar.gettime("w");
  if (w == "0") {
    return String("\202\341");
  } else if (w == "1") {
    return String("\217\255");
  } else if (w == "2") {
    return String("\202\342");
  } else if (w == "3") {
    return String("\221\340");
  } else if (w == "4") {
    return String("\227\342");
  } else if (w == "5") {
    return String("\217\342");
  } else {
    return String("\221\241");
  }
}

String rtcStr() {
  return String(rtcVar.gettime("Y-m-d H:i:s"));
}

