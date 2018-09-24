const byte DHT_SIZE = 4;
DHT dhtVar[] = {
  DHT(4, DHT22),
  DHT(7, DHT22),
  DHT(8, DHT22),
  DHT(12, DHT22),
};

void dhtSetup() {
  for (byte b = 0; b < DHT_SIZE; b++) {
    dhtVar[b].begin();
  }
}

// Температура. 7 символов
String dhtTStr(float val) {
  val = constrain(val, -99, 99);
  sprintf(char22, "%+5d.%dC\370", (int) val, ((int) (val * 10)) % 10);
  char22[7] = '\0';
  return char22;
}

// Влажность. 3 символа
String dhtHStr(float val) {
  val = constrain(val, 0, 99);
  sprintf(char22, "%2d%", (int) round(val));
  char22[3] = '\0';
  return char22;
}

// Абсолютная влажность. 6 символов
String dhtHAStr(float t, float h) {
  float val = dhtHA(t, h);
  val = constrain(val, 0, 99);
  sprintf(char22, "%2d.%dg", (int) val, ((int) (val * 100)) % 100);
  char22[6] = '\0';
  return char22;
}

float dhtH(byte b) {
  dhtVar[b].readHumidity();
}

float dhtT(byte b) {
  dhtVar[b].readTemperature();
}

float dhtHA(float t, float h) {
  if (t < -30) {
    return h * 0.1;
  } else if (t < -20) {
    return h * dhtMapFloat(t, -30, -20, 0.29, 0.81);
  } else if (t < -10) {
    return h * dhtMapFloat(t, -20, -10, 0.81, 2.1);
  } else if (t < 0) {
    return h * dhtMapFloat(t, -10, 0, 2.1, 4.8);
  } else if (t < 10) {
    return h * dhtMapFloat(t, 0, 10, 4.8, 9.4);
  } else if (t < 20) {
    return h * dhtMapFloat(t, 10, 20, 9.4, 17.3);
  } else if (t < 30) {
    return h * dhtMapFloat(t, 20, 30, 17.3, 30.4);
  } else if (t < 40) {
    return h * dhtMapFloat(t, 30, 40, 30.4, 51.1);
  } else if (t < 50) {
    return h * dhtMapFloat(t, 40, 50, 51.1, 83);
  } else if (t < 60) {
    return h * dhtMapFloat(t, 50, 60, 83, 130);
  }
  return 99;
}

float dhtMapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

