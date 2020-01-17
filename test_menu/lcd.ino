void drawMenu() {
  if (directionEncoder == -1 && id == 2) {
    return;
  }


  
  int parent = 0;             // witch level is selected
  int offset = 0;             // offset, if menu has more entries than LCD-heigh
  int currentId = id;         // save current id
  bool recalc = false;
  // no arrow in front of blind entries
  // dirty hack for first Line without Arrow in front
  if (!menuentries[id - 1].isEntry) {
    id++;
  }


  if (buttonFired) {
    // calculate new id and parent to display the correct menu
    for (int i = 0; i < SIZEOF(menuentries); i++) {
      if (menuentries[i].parent == id) {
        parent = menuentries[i].parent;
        id = menuentries[i].id;
        break;
      }
    }
    // execute menu function
    menuentries[currentId - 1].execute();
  }

  if (menuentries[id - 1].disableScroll) {
    menuentries[currentId - 1].executeRotary();
    return;
  }

  DEBUG_PRINTLN("---------");
  lcd.clear();

  // directionEncoder or button occured
  if (directionEncoder != 0 || buttonFired) {
    int newId = id + directionEncoder;
    // calculate limits prevennt overscoll
    newId = (newId < 1) ? 1 : newId;
    newId = (newId > MAXENTRIES) ? MAXENTRIES : newId;
    id = (menuentries[id - 1].parent == menuentries[newId - 1].parent) ? newId : id;
    // also calculate the offset
    for (int i = 0; i < SIZEOF(menuentries); i++) {
      if (menuentries[i].id == id) {
        parent = menuentries[i].parent;
        offset = i;
        break;
      }
    }
    // reset button flag
    buttonFired = false;
  }

  int count = 0;
  // draw menu
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    // draw only selected leven and only LCD-height count of entries
    if (i > (offset - BLOCKSIZE) && count < BLOCKSIZE && menuentries[i].parent == parent) {
      // arrow or no arrow?
      lcd.setCursor(0, count);

      if (menuentries[i].isEntry) {
        if (menuentries[i].id == id) {
          lcd.write(LCDML_DISP_cfg_cursor);
          DEBUG_PRINT("-> ");
        }
        else {
          lcd.print(" ");
          DEBUG_PRINT("   ");
        }

      }
      lcd.print(menuentries[i].lable);
      DEBUG_PRINTLN(menuentries[i].lable);
      if (menuentries[i].id == 2) {
        lcd.print(" ");
        lcd.print(menuentries[choosenProgram-1].lable);
      }
      count++;
    }
  }
  // reset directionEncoder flag
  directionEncoder = 0;
}
