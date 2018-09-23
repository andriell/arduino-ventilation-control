const byte MENU_TOP = 14;
const byte MENU_SIZE = 4;

byte menuSelected = 0;
byte menuActive = 255;




char* menuList[] = {
  "\214\256\255\250\342\256\340\250\255\243",
  "\215\240\341\342\340\256\251\252\250",
  "\204\240\342\240/\242\340\245\254\357",
  "\216\342\347\245\342",
};


void menuMenu() {
  byte prev, next;
  if (menuSelected <= 0) {
    prev = MENU_SIZE - 1;
    next = menuSelected + 1;
  } else if (menuSelected >= MENU_SIZE - 1) {
    prev = menuSelected - 1;
    next = 0;
  } else {
    prev = menuSelected - 1;
    next = menuSelected + 1;
  }

  oledPrint(menuList[prev], OLED_C, 0, 0);
  oledPrint(menuList[menuSelected], OLED_C, 3, 1);
  oledPrint(menuList[next], OLED_C, 5, 0);

  // Контроль
  if (controlE()) {
    menuOpen(menuSelected);
  }
  if (controlP()) {
    oledClean();
    menuSelected++;
    if (menuSelected >= MENU_SIZE) {
      menuSelected = 0;
    }
  }
  if (controlM()) {
    oledClean();
    if (menuSelected <= 0) {
      menuSelected = MENU_SIZE;
    }
    menuSelected--;
  }
}

void menuOpen(byte i) {
  oledClean();
  controlReset();
  menuActive = i;
  delay(200);
}

void menuTitile() {
  oledPrint(menuList[menuActive], OLED_C, 0, 0);
}

void menuLoop() {
  if (menuActive == 0) {
    infoMenu();
  } else if (menuActive == 1) {
    cfgMenu();
  } else if (menuActive == 2) {
    rtcMenu();
  } else if (menuActive == 3) {
    logMenu();
  } else {
    menuMenu();
  }
}

