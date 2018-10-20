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


float dhtT(byte b) {
  return dhtTVal[b];
}

float dhtH(byte b) {
  return dhtHVal[b];
}

float dhtTCellar() {
  if (cfgDhtInOut()) {
    return dhtTVal[0];
  }
  return dhtTVal[1];
}

float dhtHCellar() {
  if (cfgDhtInOut()) {
    return dhtHVal[0];
  }
  return dhtHVal[1];
}

float dhtTOutside() {
  if (cfgDhtInOut()) {
    return dhtTVal[1];
  }
  return dhtTVal[0];
}

float dhtHOutside() {
  if (cfgDhtInOut()) {
    return dhtHVal[1];
  }
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

