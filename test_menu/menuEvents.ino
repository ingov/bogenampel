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
    id = 11;
  }
}

void goBack(int menuId) {
  id = menuentries[menuId-1].parent;
}

void startSession(int menuId) {
  programStartet = true;
}
