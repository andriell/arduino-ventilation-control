struct ClockDate {
  byte vAddr;
  String vName;
  int vMin;
  int vMax;
  int vDivider;
  int vStep;
};

iarduino_RTC clockTime(RTC_DS1307);
bool clockShow = false;
char clockTimeStr[20];

void clockSetup() {
  clockTime.begin();
}


void clockLoop() {
  if (!clockShow) {
    return;
  }
  
}

char clocStr() {
  return clockTime.gettime("Y-m-d H:i:s");
}

