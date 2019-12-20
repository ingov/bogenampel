void updateLCD() {
  Serial.println("---------");
  lcd.clear();
  lcd.home();
  


  int parent = 0;             // witch level is selected
  int offset = 0;             // offset, if menu has more entries than LCD-heigh
  int currentId = id;         // save current id
  bool recalc = false;
  // no arrow in front of blind entries
  if (!menuentries[id - 1].isEntry) {
    id++;
  }
  Serial.println(id);


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
          Serial.print("-> ");
        }
        else {
          lcd.print(" ");
          Serial.print("   ");
        }

      }
      lcd.print(menuentries[i].lable);
      Serial.println(menuentries[i].lable);
      count++;
    }
  }
  // reset directionEncoder flag
  directionEncoder = 0;
}
