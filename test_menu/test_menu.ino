#include <Menu.h>
#define SIZEOF(m) (sizeof(m)/sizeof(m[0]))
int id = 1;
int rotation = 0;
bool buttonfired = false;
#define BLOCKSIZE 4
#define MAXENTRIES 20

/******* Program settings *********/
int choosenProgram = 8;
int choosenTimeSetting = 13;
/******* Program settings *********/

// represents the Menu-structure
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
  { 11, 3, setTime, true, "Gruppen" },
  { 12, 3, goBack, true, "zuruek" },
  { 13, 10, setTime, true, "60" },
  { 14, 10, setTime, true, "120" },
  { 15, 10, setTime, true, "180" },
  { 16, 10, setTime, true, "240" },
  { 17, 11, setTime, true, "AB" },
  { 18, 11, setTime, true, "AB/CD" },
  { 19, 4, noFunc, true, "Pause" },
  { 20, 4, noFunc, true, "Abbrechen" }
};



void setup() {
  Serial.begin(9600);
  Serial.println("test");
  updateLCD;

  buttonfired = true; // click Program
  updateLCD;

  rotation = -1; // go up
  updateLCD;

  buttonfired = true; // set Program
  updateLCD;

  rotation = 1; // go to settings
  updateLCD;

  buttonfired = true; // click settings
  updateLCD;

  buttonfired = true; // click settings
  updateLCD;

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
