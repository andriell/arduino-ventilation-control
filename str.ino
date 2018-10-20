char strChar22[22];

// Температура. 7 символов
char* strT(float val) {
  if (isnan(val)) {
    strChar22[0] = ' ';
    strChar22[1] = ' ';
    strChar22[2] = '-';
    strChar22[3] = '.';
    strChar22[4] = '-';
    strChar22[5] = '\370';
    strChar22[6] = 'C';
    strChar22[7] = '\0';
    return strChar22;
  }
  val = constrain(val, -99.0, 99.0);
  sprintf(strChar22, "%+2d.%01d\370C\0", (int) val, ((int) (val * 10.0)) % 10);
  strChar22[7] = '\0';
  return strChar22;
}

// Влажность. 3 символа
char* strH(float val) {
  if (isnan(val)) {
    strChar22[0] = '-';
    strChar22[1] = '-';
    strChar22[2] = '%';
    strChar22[3] = '\0';
    return strChar22;
  }
  val = constrain(val, 0.0, 99.0);
  sprintf(strChar22, "%2d%%\0", (int) round(val));
  strChar22[3] = '\0';
  return strChar22;
}

// Абсолютная влажность. 6 символов
char* strHA(float t, float h) {
  if (isnan(t) || isnan(h)) {
    strChar22[0] = ' ';
    strChar22[1] = '-';
    strChar22[2] = '.';
    strChar22[3] = '-';
    strChar22[4] = '-';
    strChar22[5] = 'g';
    strChar22[6] = '\0';
    return strChar22;
  }
  float val = dhtHA(t, h);
  val = constrain(val, 0.0, 99.0);
  sprintf(strChar22, "%2d.%02dg\0", (int) val, ((int) (val * 100.0)) % 100);
  strChar22[6] = '\0';
  return strChar22;
}

// 2 символа
char* str02d(int i) {
  sprintf(strChar22, "%02d\0", i);
  strChar22[2] = '\0';
  return strChar22;
}

// Месяц и день 5 символов
char* strID(int i) {
  sprintf(strChar22, "%02d-%02d\0", i / 31 + 1, i % 31 + 1);
  strChar22[5] = '\0';
  return strChar22;
}

// Часы, минуты, секунды 8 символов
char* strHMS(int ul) {
  int h = ul / 3600;
  int m = (ul - h * 3600) / 60;
  int s = ul - h * 3600 - m * 60;
  sprintf(strChar22, "%02d:%02d:%02d\0", h, m, s);
  strChar22[8] = '\0';
  return strChar22;
}

// Часы и минуты 8 символов
char* strHM(int i) {
  sprintf(strChar22, "%02d:%02d:00\0", i / 60, i % 60);
  strChar22[8] = '\0';
  return strChar22;
}

// Минуты и секунды 8 символов
char* strMS(int i) {
  sprintf(strChar22, "00:%02d:%02d\0", i / 60, i % 60);
  strChar22[8] = '\0';
  return strChar22;
}
// Первые 4 бита 4 символа
char* strB4(int i) {
  strChar22[0] = '0';
  strChar22[1] = '0';
  strChar22[2] = '0';
  strChar22[3] = '0';
  for (byte b = 0; b < 4; b++) {
    if (bitRead(i, b)) {
      strChar22[b] = '1';
    }
  }
  strChar22[4] = '\0';
  return strChar22;
}

char* strDate() {
  DateTime now = rtcNow();
  sprintf(strChar22, "%04d-%02d-%02d %02d:%02d:%02d\0", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  strChar22[19] = '\0';
  return strChar22;
}

// Последний рабочий день. 10 символов
char* strFanLastWorkDay() {
  unsigned long lastWorkDay = fanGetLastWorkDay();
  int y = lastWorkDay / 10000ul;
  int m = (lastWorkDay - y * 10000ul) / 100;
  int d = lastWorkDay - y * 10000ul - m * 100;
  sprintf(strChar22, "%04d-%02d-%02d\0", y, m, d);
  strChar22[10] = '\0';
  return strChar22;
}

// Скорость вращения вентилятора. 8 символов
char* strFanRpm() {
  sprintf(strChar22, "%5drpm\0", fanGetRpm());
  strChar22[8] = '\0';
  return strChar22;
}

// Какой это кулер in или out. 2 символа
char* strFan() {
  strChar22[0] = 'F';
  if (!fanIsRun()) {
    strChar22[1] = 'X';
  } else if (fanIsIn()) {
    strChar22[1] = 'I';
  } else {
    strChar22[1] = 'O';
  }
  strChar22[2] = '\0';
  return strChar22;
}

