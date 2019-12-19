#include <Menu.h>
#define SIZEOF(m) (sizeof(m)/sizeof(m[0]))
int id = 1;
int rotation = 0;
bool buttonfired = false;
#define BLOCKSIZE 4
#define MAXENTRIES 11

Menu menuentries [MAXENTRIES] = {
  { 1, 0, "Menu 1" },
  { 2, 0, "Menu 2" },
  { 3, 0, "Menu 3" },
  { 4, 0, "Menu 4" },
  { 5, 0, "Menu 5" },
  { 6, 0, "Menu 6" },
  { 7, 1, "Menu 1 1" },
  { 8, 1, "Menu 1 2" },
  { 9, 2, "Menu 2 1" },
  { 10, 2, "Menu 2 2" },
  { 11, 3, "Menu 3 1" }
};



void setup() {
  Serial.begin(9600);
  Serial.println("test");

  printMenu();
  rotation = 3;
  printMenu();
  rotation = 1;
  printMenu();
  rotation = 1;
  printMenu();
  rotation = -3;
  printMenu();
}


void printMenu() {
  Serial.println("---------");
  int currentParent = menuentries[id].parent;
  int newId = id + rotation;
  newId = (newId < 1) ? 1 : newId;
  newId = (newId > MAXENTRIES) ? MAXENTRIES : newId;
  id = (menuentries[id - 1].parent == menuentries[newId - 1].parent) ? newId : id;
  int parent = 0;
  int offset = 0;
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    if (menuentries[i].id == id) {
      parent = menuentries[i].parent;
      offset = i;
      break;
    }
  }
  int count = 0;
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    if (i > (offset -BLOCKSIZE) && count < BLOCKSIZE && menuentries[i].parent == parent) {
      if (menuentries[i].id == id) {
        Serial.print("-> ");
      }
      else {
        Serial.print("   ");
      }
      menuentries[i].execute();
      count++;
    }
  }
  rotation = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

}
