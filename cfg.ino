struct CfgMenuStruct {
  byte vAddr;
  String vName1;
  String vName2;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};
const byte CFG_MENU_ELEMENTS_SIZE = 16;
CfgMenuStruct cfgMenuElements[] = {
  {0, "Min T (C\370)", "", 0, 25, 1, 1},
  {1, "Max T (C\370)", "", 0, 25, 1, 1},
  {2, "Min H (%)", "", 0, 100, 1, 1},
  {3, "Max H (%)", "", 0, 100, 1, 1},
  {4, "\207\242\343\252", "", 0, 1, 1, 1},
  {5, "\215\240\347 \342\250\345\256\243\256 \340\245\246\250\254\240 (\347)", "", 0, 23, 1, 1},
  {6, "\212\256\255 \342\250\345\256\243\256 \340\245\246\250\254\240 (\347)", "", 0, 23, 1, 1},
  {7, "Mod: 0-\242\353\252\253, 1-smart", "2-\340\240\341\257, 3-smart+\340\240\341\257", 0, 3, 1, 1},
  {8, "\202\353\342\357\246\252\240 \242 00:00 (\254\250\255)", "", 0, 240, 1, 1},
  {9, "\202\353\342\357\246\252\240 \242 04:00 (\254\250\255)", "", 0, 240, 1, 1},
  {10, "\202\353\342\357\246\252\240 \242 08:00 (\254\250\255)", "", 0, 240, 1, 1},
  {11, "\202\353\342\357\246\252\240 \242 12:00 (\254\250\255)", "", 0, 240, 1, 1},
  {12, "\202\353\342\357\246\252\240 \242 16:00 (\254\250\255)", "", 0, 240, 1, 1},
  {13, "\202\353\342\357\246\252\240 \242 20:00 (\254\250\255)", "", 0, 240, 1, 1},
  {14, "\221\252\256\340\256\341\342\354 \242\245\255\342. 1", "", 0, 255, 1, 5},
  {15, "\221\252\256\340\256\341\342\354 \242\245\255\342. 2", "", 0, 255, 1, 5},
};

byte cfgAddr2I[CFG_MENU_ELEMENTS_SIZE];

void cfgSetup() {
  for (byte i = 0; i < CFG_MENU_ELEMENTS_SIZE; i++) {
    cfgAddr2I[cfgMenuElements[i].vAddr] = i;
  }
  //for (byte i = 0; i < CFG_MENU_ELEMENTS_SIZE; i++) {
  //  Serial.print(i);
  //  Serial.print(" ");
  //  Serial.println(cfgAddr2I[i]);
  //}
}

byte cfgMenuI = 0;
bool cfgMenuEnter = false;

void cfgMenu() {
  menuTitile();
  CfgMenuStruct current = cfgMenuElements[cfgMenuI];
  int valInt = cfgRead(current.vAddr);
  
  if (cfgMenuEnter) {
    oledInvText(true);
  }
  if (current.vDivider > 1) {
    float valFloat = ((float) valInt) / current.vDivider;
    oledPrintFloat(valFloat, OLED_C, 3, 1);
  } else if (current.vMin == 0 && current.vMax == 1) {
    if (valInt > 0) {
      oledPrint("\204\240", OLED_C, 3, 1);
    } else {
      oledPrint("\215\245\342", OLED_C, 3, 1);
    }
  } else {
    oledPrintInt(valInt, OLED_C, 3, 1);
  }
  oledInvText(false);

  oledPrint(current.vName1, OLED_C, 5, 0);
  oledPrint(current.vName2, OLED_C, 6, 0);

  // Контроль
  if (controlE()) {
    oledClean();
    cfgMenuEnter = true;
  }
  if (controlC()) {
    if (cfgMenuEnter) {
      oledClean();
      cfgMenuEnter = false;
    } else {
      menuOpen(255);
    }
  }
  if (controlP()) {
    oledClean();
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
    oledClean();
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
    val = cfgMenuElements[cfgAddr2I[addr]].vMax;
  }
  if (val > cfgMenuElements[cfgAddr2I[addr]].vMax) {
    val = cfgMenuElements[cfgAddr2I[addr]].vMin;
  }
  EEPROM.put(addr * 2, val);
}

float cfgGetMinT() {
  return cfgReadFloat(0);
}
float cfgGetMaxT() {
  return cfgReadFloat(1);
}
float cfgGetMInH() {
  return cfgReadFloat(2);
}
float cfgGetMaxH() {
  return cfgReadFloat(3);
}
int cfgGetSound() {
  return cfgRead(4);
}
int cfgGetSoundStart() {
  return cfgRead(5);
}
int cfgGetSoundEnd() {
  return cfgRead(6);
}
int cfgGetMod() {
  return cfgRead(7);
}
int cfgGetRun00() {
  return cfgRead(8);
}
int cfgGetRun04() {
  return cfgRead(9);
}
int cfgGetRun08() {
  return cfgRead(10);
}
int cfgGetRun12() {
  return cfgRead(11);
}
int cfgGetRun16() {
  return cfgRead(12);
}
int cfgGetRun20() {
  return cfgRead(13);
}
int cfgGetSpeedFan1() {
  return cfgRead(14);
}
int cfgGetSpeedFan2() {
  return cfgRead(15);
}

