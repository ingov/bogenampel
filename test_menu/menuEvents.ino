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
void setTime(int menuId) {
  if (menuId == 10) {
    id = choosenTimeSetting;
  }
  else {
    choosenTimeSetting = menuId;
    id = 10;
  }
}

void setGroup(int menuId) {
  if (menuId == 11) {
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
    id = 11;
  }

}

void goBack(int menuId) {
  id = menuentries[menuId - 1].parent;
}

void startSession(int menuId) {
  sendToClinet();
  programStartet = true;
}

void sendToClinet() {
  String txData = String(groups[programLaps%2]);
  char foo[3];
  sprintf(foo, "%03d", menuentries[choosenTimeSetting - 1].lable.toInt());
  txData += foo;
  DEBUG_PRINT("SEND: ");
  DEBUG_PRINTLN(txData);
}
