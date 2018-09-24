
iarduino_OLED_txt oledVar(0x3C);

// ширина символов (6), высота символов (8),
extern uint8_t SmallFontRus[];
// ширина символов (12), высота символов (16)
extern uint8_t MediumFontRus[];
// Предыдущий размер шрифта
byte oledFSPrev = 255;

void oledSetap() {
  oledVar.begin();
  //oledFlipDisplay();
  oledVar.setCoding(false);
  oledClean();
  oledPrint("OLED\0", OLED_C, 4, 1);
}

void oledFont(byte fs) {
  if (fs == oledFSPrev) {
    return;
  }
  if (fs == 0) {
    oledVar.setFont(SmallFontRus);
  } else if (fs == 1) {
    oledVar.setFont(MediumFontRus);
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


/*void oledNormalDisplay()   {
  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xC8);
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xA1);
  Wire.endTransmission();
}

void oledFlipDisplay() {
  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xC0);
  Wire.endTransmission();

  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x80);
  Wire.write(0xA0);
  Wire.endTransmission();
}*/

