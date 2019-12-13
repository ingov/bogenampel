String menu[] = {"Program: ", "Program aendern", "Program starten"};
String programme[] = {"WA 18/3", "WA 30/60/90", "WA 0815", "WA ..."};
String settings[] = {"Zeit", "Gruppen", "Runden", "zuruek"};
String startOptions[] = {"Weiter", "Stop"};
String gruppen[] = {"AB", "AB/CD"};

int menuSelection = 0;
int program = 0;
int setting = 0;
int startoption = 0;
int gruppe = 0;

int currentPointer = 0;

int menuCount = 3;
int programCount = 4;
int settingsCount = 4;
int startOptionsCount = 2;
int gruppenCount = 2;

void updateLcd() {
  if (buttonFired) {

  }
  int maxEntries = 0;
  switch (menuSelection) {
    case 0:
      maxEntries = menuCount;
      if (buttonFired) {
        menuSelection = currentPointer + 1;
        currentPointer = 0;
      }
      break;
    case 1:
      maxEntries = programCount;
      if (buttonFired) {
        program = currentPointer;
        menuSelection = 0;
        currentPointer = 0;
      }
      break;
    case 2:
      maxEntries = settingsCount;
      if (buttonFired) {
        switch (currentPointer) {
          case 0:
            // zeit
            break;
          case 1:
            menuSelection = 4;
            currentPointer = 0;
            break;
          case 2:
            // runden
            break;
          case 3:
            menuSelection = 0;
            currentPointer = 1;
            break;
        }
      }
      break;
    case 3:
      Serial.println("case3");
      maxEntries = startOptionsCount;
      break;
    case 4:
      if (buttonFired) {
        gruppe = currentPointer + 1;
        currentPointer = 1;
        menuSelection = 2;
      }
      maxEntries = startOptionsCount;
      break;
  }

  if (directionEncoder == 1) {
    currentPointer ++;
    if (currentPointer == maxEntries)
      currentPointer = 0;
  }
  else if (directionEncoder == 2) {
    currentPointer --;
    if (currentPointer < 0)
      currentPointer = maxEntries - 1;
  }
  drawLcd();
  Serial.print("currentPointer: ");
  Serial.println(currentPointer);


}

void drawLcd() {
  lcd.clear();
  switch (menuSelection) {
    case 0:
      for (int i = 0; i < menuCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(_LCDML_DISP_cfg_cursor);
        else
          lcd.print(" ");
        lcd.print(menu[i]);
        if (i == 0)
          lcd.print(programme[program]);
      }
      break;
    case 1:
      for (int i = 0; i < programCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(_LCDML_DISP_cfg_cursor);
        else
          lcd.print(" ");
        lcd.print(programme[i]);
      }
      break;
    case 2:
      for (int i = 0; i < settingsCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(_LCDML_DISP_cfg_cursor);
        else
          lcd.print(" ");
        lcd.print(settings[i]);
      }
      break;
    case 3:
        lcd.setCursor(0, 0);
        lcd.print(programme[program]);
        lcd.setCursor(0, 1);
        lcd.print(gruppen[gruppe]);

      for (int i = 0; i < startOptionsCount; i++) {
        lcd.setCursor(0, i+2);
        if (i == currentPointer)
          lcd.write(_LCDML_DISP_cfg_cursor);
        else
          lcd.print(" ");
        lcd.print(startOptions[i]);
      }
      break;
    case 4:
      for (int i = 0; i < gruppenCount; i++) {
        lcd.setCursor(0, i);
        if (i == currentPointer)
          lcd.write(_LCDML_DISP_cfg_cursor);
        else
          lcd.print(" ");
        lcd.print(gruppen[i]);
      }
      break;
  }
}
