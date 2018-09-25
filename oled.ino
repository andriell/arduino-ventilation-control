#define OLED_ADDRESS 0x3C
iarduino_OLED_txt oledVar(OLED_ADDRESS);

// ширина символов (6), высота символов (8),
extern uint8_t SmallFontRus[];
// ширина символов (12), высота символов (16)

// Предыдущий размер шрифта
byte oledFSPrev = 255;

void oledSetap() {
  oledVar.begin();
  oledFlipDisplay();
  oledVar.setCoding(false);
  oledClean();
  oledPrint("OLED\0", OLED_C, 4, 0);
}

void oledFont(byte fs) {
  if (fs == oledFSPrev) {
    return;
  }
  if (fs == 0) {
    oledVar.setFont(SmallFontRus);
  } else if (fs == 1) {
    oledVar.setFont(MediumFont36);
  }
  fs = oledFSPrev;
}

void oledClean() {
  oledVar.clrScr();
}

void oledPrint(char* txt, int x, int y, byte fs) {
  oledFont(fs);
  oledVar.print(txt, x, y);
}

void oledPrintByte(byte txt, int x, int y, byte fs) {
  oledFont(fs);
  oledVar.print(txt, x, y);
}

void oledPrintInt(int txt, int x, int y, byte fs) {
  oledFont(fs);
  oledVar.print(txt, x, y);
}

void oledPrintFloat(float txt, int x, int y, byte fs) {
  oledFont(fs);
  oledVar.print(txt, x, y);
}
void oledInvText(bool inv) {
  oledVar.invText(inv);
}


void oledNormalDisplay()   {
  /*Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xC8);
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xA1);
  Wire.endTransmission();*/
  // Библиотека iarduino_OLED_txt не дружит с Wire поэтому в файле iarduino_OLED_txt.h метод _sendCommand нужно сделать публичным
  oledVar._sendCommand(0xC8);
  oledVar._sendCommand(0xA1);
}

void oledFlipDisplay() {
  /*Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xC0);
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xA0);
  Wire.endTransmission();*/
  // Библиотека iarduino_OLED_txt не дружит с Wire поэтому в файле iarduino_OLED_txt.h метод _sendCommand нужно сделать публичным
  oledVar._sendCommand(0xC0);
  oledVar._sendCommand(0xA0);
}

