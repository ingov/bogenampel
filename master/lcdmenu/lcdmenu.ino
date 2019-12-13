#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// *********************************************************************
// LCDML display settings
// *********************************************************************
// settings for LCD
#define _LCDML_DISP_cols  20
#define _LCDML_DISP_rows  4

#define _LCDML_DISP_cfg_cursor                     0x7E   // cursor Symbol

LiquidCrystal_I2C lcd(0x27, _LCDML_DISP_cols, _LCDML_DISP_rows);

String menu[] = {"Program: ", "Program aendern", "Program starten"};
String programme[] = {"WA 18 / 3", "WA 30 / 60 / 90", "WA 0815", "WA ..."};
String settings[] = {"Zeit", "Runden"};
String start[] = {"Weiter", "Stop"};

int menuSelection = 0;
int program = 0;
int setting = 0;
int start = 0;
int currentPointer = 0;

int menuCount = 3;
int programCount = 4;
int settingsCount = 2;
int startCount = 2;


bool menuChange = false;
bool buttonFired = false;

void setup() {

  // serial init; only be needed if serial control is used
  Serial.begin(9600);                // start serial
  // LCD Begin
  lcd.init();
  lcd.backlight();
  drawLcd();
  delay(800);
}

void loop() {
  currentPointer = 0;
  menuSelection = 0;
  drawLcd();
  delay(1000);
  currentPointer = 0;
  menuSelection = 1;
  drawLcd();
  delay(900);
  currentPointer = 1;
  drawLcd();
  delay(900);
  currentPointer = 2;
  drawLcd();
  delay(1000);
  program = 2;
  currentPointer = 0;
  menuSelection = 0;
  drawLcd();
  delay(1000);
  currentPointer = 1;
  drawLcd();
  delay(1000);
  currentPointer = 0;
  menuSelection = 2;
  drawLcd();
  delay(2000);
  
  

}

void drawLcd() {
  lcd.clear();
  switch (menuSelection) {
    case 0:
      for (int i = 0; i < menuCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(126);
        else
          lcd.print(" ");
        lcd.print(menu[i]);
        if (i==0)
        lcd.print(programme[program]);
      }
      break;
    case 1:
      for (int i = 0; i < programCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(126);
        else
          lcd.print(" ");
        lcd.print(programme[i]);
      }
      break;
    case 2:
      for (int i = 0; i < settingsCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(126);
        else
          lcd.print(" ");
        lcd.print(settings[i]);
      }
      break;
  }
}
