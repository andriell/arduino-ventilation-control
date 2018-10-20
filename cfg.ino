#define CFG_MD 371
#define CFG_HM 1439
#define CFG_MS 3599
#define CFG_B16 15

struct CfgMenuStruct {
  byte vAddr;
  char* vName1;
  char* vName2;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};

const byte CFG_MENU_ELEMENTS_SIZE = 17;

byte cfgAddr2I[CFG_MENU_ELEMENTS_SIZE];

CfgMenuStruct cfgMenuElements[] = {
  {0, "Min T (C\370)\0", "\0", 0, 25, 1, 1},
  {1, "Max T (C\370)\0", "\0", 0, 25, 1, 1},
  {2, "Min H (%)\0", "\0", 0, 100, 1, 1},
  {3, "Max H (%)\0", "\0", 0, 100, 1, 1},
  {4, "\207\242\343\252\0", "\0", 0, 1, 1, 1},
  {5, "\215\240\347 \342\250\345\256\243\256 \340\245\246\250\254\240 (\347)\0", "\0", 0, CFG_HM, 1, 10},
  {6, "\212\256\255 \342\250\345\256\243\256 \340\245\246\250\254\240 (\347)\0", "\0", 0, CFG_HM, 1, 10},
  {7, "\215\240\347 \340\240\241 \257\256 \342\245\254\257\0", "\0", 0, CFG_MD, 1, 1},
  {8, "\212\256\255 \340\240\241 \257\256 \342\245\254\257\0", "\0", 0, CFG_MD, 1, 1},
  {9, "\215\240\347 \340\240\241 \257\256 \242\253\240\246\0", "\0", 0, CFG_MD, 1, 1},
  {10, "\212\256\255 \340\240\241 \257\256 \242\253\240\246\0", "\0", 0, CFG_MD, 1, 1},
  {11, "\220\240\341\257\250\341\240\255\250\245, \341\343\345\256\245\0", "\242\340\245\254\357\0", 0, CFG_HM, 1, 10},
  {12, "\220\240\341\257\250\341\240\255\250\245, \250\255\342\245\340\242\240\253\0", "(\244\245\255\354)\0", 1, 10, 1, 1},
  {13, "\220\240\341\257\250\341\240\255\250\245, \242\340\245\254\357\0", "\340\240\241\256\342\353 (\341\245\252)\0", 0, CFG_MS, 1, 30},
  {14, "\217\245\340\245\252\253\356\347\245\255\250\357 \254\245\255\356\0", "(\341\245\252)\0", 1, 10, 1, 1},
  {15, "\220\245\246\250\254 \340\240\241\256\342\353\0", "TE HU TE+HU SC\0", 0, CFG_B16, 1, 1},
  {16, "\202\340\245\254\357 \257\245\340\245\252\253\356\347\245\255\250\357\0", "\242\245\255\342\250\253\357\342\256\340\256\242\0", 60, CFG_MS, 1, 30},
};

void cfgSetup() {
  for (byte i = 0; i < CFG_MENU_ELEMENTS_SIZE; i++) {
    cfgAddr2I[cfgMenuElements[i].vAddr] = i;
  }
  //for (byte i = 0; i < CFG_MENU_ELEMENTS_SIZE; i++) {
  //  Serial.print(i);
  //  Serial.print(" \0");
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
  // Выводим изменяемую величину
  if (current.vDivider > 1) {
    // float
    float valFloat = ((float) valInt) / current.vDivider;
    oledPrintFloat(valFloat, OLED_C, 3, 1);
  } else if (current.vMin == 0 && current.vMax == 1) {
    // bool
    if (valInt > 0) {
      oledPrint("+\0", OLED_C, 3, 1);
    } else {
      oledPrint("-\0", OLED_C, 3, 1);
    }
  } else if (current.vMax == CFG_HM) {
    // Часы и минуты
    oledPrint(strHM(valInt), OLED_C, 3, 1);
  } else if (current.vMax == CFG_MS) {
    // Минуты и секунды
    oledPrint(strMS(valInt), OLED_C, 3, 1);
  } else if (current.vMax == CFG_MD) {
    // Месяц и день
    oledPrint(strID(valInt), OLED_C, 3, 1);
  } else if (current.vMin == 0 && current.vMax == CFG_B16) {
    // Byte
    oledPrint(strB4(valInt), OLED_C, 3, 1);
  } else {
    // int
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
  return constrain(val, cfgMenuElements[cfgAddr2I[addr]].vMin, cfgMenuElements[cfgAddr2I[addr]].vMax);
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
float cfgGetMinH() {
  return cfgReadFloat(2);
}
float cfgGetMaxH() {
  return cfgReadFloat(3);
}
int cfgGetSound() {
  return cfgRead(4);
}
int cfgGetSoundStartHM() {
  return cfgRead(5);
}
int cfgGetSoundEndHM() {
  return cfgRead(6);
}
int cfgGetTModStartMD() {
  return cfgRead(7);
}
int cfgGetTModEndMD() {
  return cfgRead(8);
}
int cfgGetHModStartMD() {
  return cfgRead(9);
}
int cfgGetHModEndMD() {
  return cfgRead(10);
}
int cfgGetDryTimeHM() {
  return cfgRead(11);
}
int cfgGetDryTimeIntervalDay() {
  return cfgRead(12);
}
int cfgGetDryTimeRunTimeS() {
  return cfgRead(13);
}
int cfgGetRotationSec() {
  return cfgRead(14);
}
int cfgMod() {
  return cfgRead(15);
}
int cfgFanRotationTime() {
  return cfgRead(16);
}

