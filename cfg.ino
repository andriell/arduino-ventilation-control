struct CfgMenuStruct {
  byte vAddr;
  String vName;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};
const byte CFG_MENU_ELEMENTS_SIZE = 4;
CfgMenuStruct cfgMenuElements[] = {
  {0, "Min T (C\370)", 0, 250, 10, 1},
  {1, "Max T (C\370)", 0, 250, 10, 1},
  {2, "Min H (%)", 0, 100, 1, 1},
  {3, "Max H (%)", 0, 100, 1, 1},
};

byte cfgAddr2I[CFG_MENU_ELEMENTS_SIZE];

void cfgSetup() {
  for (byte i = 0; i < CFG_MENU_ELEMENTS_SIZE; i++) {
    cfgAddr2I[cfgMenuElements[i].vAddr] = i;
  }
  for (byte i = 0; i < 20; i++) {
    Serial.print(i);
    Serial.print(" ");
    Serial.println(cfgAddr2I[i]);
  }
}
bool cfgMenuShow = false;
byte cfgMenuI = 0;
bool cfgMenuEnter = false;

void cfgLoop() {
  if (!cfgMenuShow) {
    return;
  }
  CfgMenuStruct current = cfgMenuElements[cfgMenuI];
  int valInt = cfgRead(current.vAddr);
  float val = ((float) valInt) / current.vDivider;
  if (cfgMenuEnter) {
    oledPrint(">", 20, 30, 1);
  }
  oledPrintFloat(val, 28, 30, 1);

  oledPrintNl(current.vName, 40);

  // Контроль
  if (controlE()) {
    cfgMenuEnter = true;
  }
  if (controlC()) {
    if (!cfgMenuEnter) {
      
    }
    cfgMenuEnter = false;
  }
  if (controlP()) {
    if (cfgMenuEnter) {
      cfgWrite(current.vAddr, valInt + current.vStep);
    } else {
      cfgMenuI++;
      if (cfgMenuI >= CFG_MENU_ELEMENTS_SIZE - 1) {
        cfgMenuI = 0;
      }
    }
  }
  if (controlM()) {
    if (cfgMenuEnter) {
      cfgWrite(current.vAddr, valInt - current.vStep);
    } else {
      if (cfgMenuI <= 0) {
        cfgMenuI = CFG_MENU_ELEMENTS_SIZE;
      }
      cfgMenuI--;
    }
  }
}

// чтение
int cfgRead(byte addr) {
  int val;
  EEPROM.get(addr * 2, val);
  if (val < cfgMenuElements[cfgAddr2I[addr]].vMin) {
    return cfgMenuElements[cfgAddr2I[addr]].vMin;
  }
  if (val > cfgMenuElements[cfgAddr2I[addr]].vMax) {
    return cfgMenuElements[cfgAddr2I[addr]].vMax;
  }
  return val;
}

float cfgReadFloat(byte addr) {
  return ((float) cfgRead(addr)) / cfgMenuElements[cfgAddr2I[addr]].vDivider;
}

// запись
void cfgWrite(byte addr, int val) {
  if (val < cfgMenuElements[cfgAddr2I[addr]].vMin) {
    val = cfgMenuElements[cfgAddr2I[addr]].vMin;
  }
  if (val > cfgMenuElements[cfgAddr2I[addr]].vMax) {
    val = cfgMenuElements[cfgAddr2I[addr]].vMax;
  }
  EEPROM.put(addr * 2, val);
}

