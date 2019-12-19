void updateLCD() {
  Serial.println("---------");

  int parent = 0;             // witch level is selected
  int offset = 0;             // offset, if menu has more entries than LCD-heigh
  int currentId = id;         // save current id
  bool recalc = false;
  // no arrow in front of blind entries
  if (!menuentries[id - 1].isEntry) {
    id++;
  }
  Serial.println(id);


  if (buttonfired) {
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


  // rotation or button occured
  if (rotation != 0 || buttonfired) {
    int newId = id + rotation;
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
    buttonfired = false;
  }

  int count = 0;
  // draw menu
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    // draw only selected leven and only LCD-height count of entries
    if (i > (offset - BLOCKSIZE) && count < BLOCKSIZE && menuentries[i].parent == parent) {
      // arrow or no arrow?
      if (menuentries[i].isEntry) {
        if (menuentries[i].id == id) {
          Serial.print("-> ");
        }
        else {
          Serial.print("   ");
        }

      }
      Serial.println(menuentries[i].lable);
      count++;
    }
  }
  // reset rotation flag
  rotation = 0;
}
