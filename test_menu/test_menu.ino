#include <Menu.h>
#define SIZEOF(m) (sizeof(m)/sizeof(m[0]))
int id = 1;
int rotation = 0;
bool buttonfired = false;
#define BLOCKSIZE 4
#define MAXENTRIES 18

/******* Program settings *********/
int choosenProgram = 8;
int choosenTimeSetting = 13;
/******* Program settings *********/


Menu menuentries [MAXENTRIES] = {
  { 1, 0, noFunc, false, "Hauptmenu" },
  { 2, 0, selectProgram, true, "Program" },
  { 3, 0, noFunc, true, "Program Einstellung" },
  { 4, 0, noFunc, true, "Program starten" },
  { 5, 2, setProgram, true, "Program 1" },
  { 6, 2, setProgram, true, "Program 2" },
  { 7, 2, setProgram, true, "Program 3" },
  { 8, 2, setProgram, true, "Program 4" },
  { 9, 2, setProgram, true, "Program 5" },
  { 10, 3, setTime, true, "Zeiteinstellung" },
  { 11, 3, noFunc, true, "Runden" },
  { 12, 3, goBack, true, "zuruek" },
  { 13, 10, setTime, true, "60" },
  { 14, 10, setTime, true, "120" },
  { 15, 10, setTime, true, "180" },
  { 16, 10, setTime, true, "240" },
  { 17, 4, noFunc, true, "Pause" },
  { 18, 4, noFunc, true, "Abbrechen" }
};



void setup() {
  Serial.begin(9600);
  Serial.println("test");
  printMenu();

  buttonfired = true; // click Program
  printMenu();

  rotation = -1; // go up
  printMenu();

  buttonfired = true; // set Program
  printMenu();

  rotation = 1; // go to settings
  printMenu();

  buttonfired = true; // click settings
  printMenu();

  buttonfired = true; // click settings
  printMenu();

}


void printMenu() {
  Serial.println("---------");

  int parent = 0;
  int offset = 0;
  int currentId = id;
  bool recalc = false;
    if (!menuentries[id - 1].isEntry) {
      id++;
    }
    Serial.println(id);

  
  if (buttonfired) {
    for (int i = 0; i < SIZEOF(menuentries); i++) {
      if (menuentries[i].parent == id) {
        parent = menuentries[i].parent;
        id = menuentries[i].id;
        break;
      }
    }

    menuentries[currentId - 1].execute();
  }

  if (rotation != 0 || buttonfired) {
    int newId = id + rotation;
    newId = (newId < 1) ? 1 : newId;
    newId = (newId > MAXENTRIES) ? MAXENTRIES : newId;
    id = (menuentries[id - 1].parent == menuentries[newId - 1].parent) ? newId : id;
    for (int i = 0; i < SIZEOF(menuentries); i++) {
      if (menuentries[i].id == id) {
        parent = menuentries[i].parent;
        offset = i;
        break;
      }
    }
    buttonfired = false;
  }

  int count = 0;
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    if (i > (offset - BLOCKSIZE) && count < BLOCKSIZE && menuentries[i].parent == parent) {

      if (menuentries[i].isEntry) {
        if (menuentries[i].id == id) {
          Serial.print("-> ");
        }
        else {
          Serial.print("   ");
        }

      }
      Serial.println(menuentries[i].lable);
      //execute();
      count++;
    }
  }
  rotation = 0;
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

void goBack(int menuId) {
  id = menuentries[menuId-1].parent;
}

void loop() {
  // put your main code here, to run repeatedly:

}
