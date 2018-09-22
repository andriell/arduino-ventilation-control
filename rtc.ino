struct ClockDate {
  byte vAddr;
  String vName;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};

iarduino_RTC rtcVar(RTC_DS1307);
bool rtcShow = false;

void rtcSetup() {
  rtcVar.begin();
}


void rtcLoop() {
  if (!rtcShow) {
    return;
  }
  
}

void rtcMenu() {
  menuTitile();
  if (controlC()) {
    menuOpen(255);
  }
}

char rtcStr() {
  return rtcVar.gettime("Y-m-d H:i:s");
}

