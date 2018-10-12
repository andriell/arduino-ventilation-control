byte runReturnMenu = 0;
int runMin = 30;
unsigned long runCloseUnixtime = 0ul;


void runOpenMenu() {
  runReturnMenu = menuGetActive();
  menuOpen(5);
  runCloseUnixtime = rtcUnixtime() + 5ul;
  runMin = 30;
}

void runMenu() {
  oledPrint("\202\252\253\356\347\250\342\354 \242\353\342\357\246\252\343\0", OLED_C, 0, 0);
  oledPrint(strHM(runMin), OLED_C, 3, 1);
  
  // Контроль
  if (controlC() || controlE() || rtcUnixtime() >= runCloseUnixtime) {
    menuOpen(runReturnMenu);
    fanRunAll(((unsigned long)runMin) * 60ul + rtcUnixtime());
  }
  if (controlP()) {
    oledClean();
    runMin += 10;
    if (runMin > 360) {
      runMin = 0;
    }
    runCloseUnixtime = rtcUnixtime() + 5ul;
  }
  if (controlM()) {
    oledClean();
    runMin -= 10;
    if (runMin < 0) {
      runMin = 360;
    }
    runCloseUnixtime = rtcUnixtime() + 5ul;
  }
}
