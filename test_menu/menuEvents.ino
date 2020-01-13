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
  else if (directionEncoder == -1 && preShootSetting > 1) {
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
  if (menuId == 10) {
    id = choosenTimeSetting;
  }
  else {
    choosenTimeSetting = menuId;
    id = 10;
  }
}
void setPreShootTime(int menuId) {
  if (menuId == 11) {
    id = choosenPreShootSetting;
  }
  else {
    choosenPreShootSetting = menuId;
    id = 11;
  }
}

void setGroup(int menuId) {
  if (menuId == 12) {
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
    id = 12;
  }

}

void goBack(int menuId) {
  id = menuentries[menuId - 1].parent;
}

void startSession(int menuId) {
  programStartet = sendToClinet(0);
}

bool sendToClinet(int mode) {
  String txData;
  DEBUG_PRINTLN(programLaps);
  if (mode == 0) {
    txData = String(groups[programLaps % 2]);
    char formatter3[3];
    char formatter2[2];
    sprintf(formatter3, "%03d", timeSetting);
    sprintf(formatter2, "%02d", preShootSetting);
    txData += formatter3;
    txData += formatter2;
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
  char sendingArray[10];
  txData.toCharArray(sendingArray, 10);

  radio.stopListening(); // stop listening and start sending
  return radio.write( &sendingArray, sizeof(sendingArray));

}
