void buttonClicked() {
  buttonFired = true;
}

void selectProgram(int menuId) {
  id = choosenProgram;
}

void setProgram(int menuId) {
  choosenProgram = menuId;
  id = 2;
}

void timeTrigger() {
  if (directionEncoder == 1 && timeSetting < timeSettingMax) {
    timeSetting += timeSettingStep;
  }
  else if (directionEncoder == -1 && timeSetting > 10) {
    timeSetting -= timeSettingStep;
  }
  if (directionEncoder == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menuentries[id - 1].lable);
    lcd.setCursor(0, 1);
    lcd.print(timeSetting);
  }
  lcd.setCursor(0, 1);
  lcd.print(timeSetting);
  lcd.print("  ");
}
void preShotTrigger() {
  if (directionEncoder == 1 && preShootSetting < preShootSettingMax) {
    preShootSetting += preShootSettingStep;
  }
  else if (directionEncoder == -1 && preShootSetting > 0) {
    preShootSetting -= preShootSettingStep;
  }
  if (directionEncoder == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menuentries[id - 1].lable);
    lcd.setCursor(0, 1);
    lcd.print(preShootSetting);
  }
  lcd.setCursor(0, 1);
  lcd.print(preShootSetting);
  lcd.print("  ");

}

void setTime(int menuId) {
  if (menuId == 11) {
    id = choosenTimeSetting;
  }
  else {
    choosenTimeSetting = menuId;
    id = 11;
  }
}
void setPreShootTime(int menuId) {
  if (menuId == 10) {
    id = choosenPreShootSetting;
  }
  else {
    choosenPreShootSetting = menuId;
    id = 10;
  }
}

void yellowLEDTrigger() {
   if (directionEncoder == 1 && yellowLEDSetting < yellowLEDSettingMax) {
    yellowLEDSetting += yellowLEDSettingStep;
  }
  else if (directionEncoder == -1 && yellowLEDSetting > 5) {
    yellowLEDSetting -= yellowLEDSettingStep;
  }
  if (directionEncoder == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menuentries[id - 1].lable);
    lcd.setCursor(0, 1);
    lcd.print(yellowLEDSetting);
  }
  lcd.setCursor(0, 1);
  lcd.print(yellowLEDSetting);
  lcd.print("  ");
 
}
void setYellowLEDTime(int menuId){
    if (menuId == 12) {
    id = choosenYellowLEDSetting;
  }
  else {
    choosenYellowLEDSetting = menuId;
    id = 12;
  }
}

void setGroup(int menuId) {
  if (menuId == 13) {
    id = choosenGroupSetting;
  }

  else {
    choosenGroupSetting = menuId;
    if (String(menuentries[choosenGroupSetting - 1].lable).equals("AB")) {
      groups[1] = 'A';
    }
    else {
      groups[1] = 'C';
    }
    id = 13;
  }

}

void goBack(int menuId) {
  id = menuentries[menuId - 1].parent;
}

void saveSettings(int menuId) {
  saveConfigToSD();
  id = 3;
}

void startSession(int menuId) {
  programStartet = sendToClinet(0);
}

bool sendToClinet(int mode) {
  String txData;
  if (mode == 0) {
    txData = String(groups[(programLaps + groupCounter % 2) % 2]);
    char timeFormat[9];
    sprintf(timeFormat, "%03d%02d%02d%02d", timeSetting, preShootSetting, yellowLEDSetting, choosenGroupSetting);
    txData += timeFormat;
  }
  else if (mode == 1) {
    txData = "PAUSE";
  }
  else if (mode == 2) {
    txData = "STOP";
  }
  else {
    txData = "NULL";
  }

  DEBUG_PRINT("SEND: ");
  DEBUG_PRINTLN(txData);
  char sendingArray[15];
  txData.toCharArray(sendingArray, 15);

  radio.stopListening(); // stop listening and start sending
  return radio.write( &sendingArray, sizeof(sendingArray));

}
