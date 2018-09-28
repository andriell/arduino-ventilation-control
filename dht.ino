const byte DHT_SIZE = 2;
DHT dhtVar[] = {
  DHT(4, DHT22),
  DHT(7, DHT22),
  //DHT(8, DHT22),
  //DHT(12, DHT22),
};

float dhtTVal[4] = {0, 0, 0, 0};
float dhtHVal[4] = {0, 0, 0, 0};

void dhtSetup() {
  for (byte b = 0; b < DHT_SIZE; b++) {
    dhtVar[b].begin();
  }
}

void dthLoop() {
  for (byte b = 0; b < DHT_SIZE; b++) {
    dhtTVal[b] = dhtVar[b].readTemperature();
    dhtHVal[b] = dhtVar[b].readHumidity();
  }
}

// Температура. 7 символов
char* dhtTStr(float val) {
  if (isnan(val)) {
    char22[0] = ' ';
    char22[1] = ' ';
    char22[2] = '-';
    char22[3] = '.';
    char22[4] = '-';
    char22[5] = '\370';
    char22[6] = 'C';
    char22[7] = '\0';
    return char22;
  }
  val = constrain(val, -99.0, 99.0);
  sprintf(char22, "%+2d.%01d\370C\0", (int) val, ((int) (val * 10.0)) % 10);
  char22[7] = '\0';
  return char22;
}

// Влажность. 3 символа
char* dhtHStr(float val) {
  if (isnan(val)) {
    char22[0] = '-';
    char22[1] = '-';
    char22[2] = '%';
    char22[3] = '\0';
    return char22;
  }
  val = constrain(val, 0.0, 99.0);
  sprintf(char22, "%2d%%\0", (int) round(val));
  char22[3] = '\0';
  return char22;
}

// Абсолютная влажность. 6 символов
char* dhtHAStr(float t, float h) {
  if (isnan(t) || isnan(h)) {
    char22[0] = ' ';
    char22[1] = '-';
    char22[2] = '.';
    char22[3] = '-';
    char22[4] = '-';
    char22[5] = 'g';
    char22[6] = '\0';
    return char22;
  }
  float val = dhtHA(t, h);
  val = constrain(val, 0.0, 99.0);
  sprintf(char22, "%2d.%02dg\0", (int) val, ((int) (val * 100.0)) % 100);
  char22[6] = '\0';
  return char22;
}

float dhtT(byte b) {
  return dhtTVal[b];
}

float dhtH(byte b) {
  return dhtHVal[b];
}

float dhtTCellar() {
  return dhtTVal[0];
}

float dhtHCellar() {
  return dhtHVal[0];
}

float dhtTOutside() {
  return dhtTVal[1];
}

float dhtHOutside() {
  return dhtHVal[1];
}
// Расчитывает абсолютную влажность
float dhtHA(float t, float h) {
  h = h / 100.0;
  if (t < -30.0) {
    return h * 0.1;
  } else if (t < -20.0) {
    return h * dhtMapFloat(t, -30.0, -20.0, 0.29, 0.81);
  } else if (t < -10.0) {
    return h * dhtMapFloat(t, -20.0, -10.0, 0.81, 2.1);
  } else if (t < 0.0) {
    return h * dhtMapFloat(t, -10.0, 0.0, 2.1, 4.8);
  } else if (t < 10.0) {
    return h * dhtMapFloat(t, 0.0, 10.0, 4.8, 9.4);
  } else if (t < 20.0) {
    return h * dhtMapFloat(t, 10.0, 20.0, 9.4, 17.3);
  } else if (t < 30.0) {
    return h * dhtMapFloat(t, 20.0, 30.0, 17.3, 30.4);
  } else if (t < 40.0) {
    return h * dhtMapFloat(t, 30.0, 40.0, 30.4, 51.1);
  } else if (t < 50.0) {
    return h * dhtMapFloat(t, 40.0, 50.0, 51.1, 83.0);
  } else if (t < 60.0) {
    return h * dhtMapFloat(t, 50.0, 60.0, 83.0, 130.0);
  }
  return 99.0;
}

float dhtMapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Какой будет относительная влажность, если температура изменится с t1 до t2
float dhtHChange(float h, float t1, float t2) {
  if (t1 == t2) {
    return h;
  }
  return constrain(dhtHA(t1, h) / dhtHA(t2, 100.0) * 100.0, 0.0, 99.0);
}

