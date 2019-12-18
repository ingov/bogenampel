#include <Menu.h>
#define SIZEOF(m) (sizeof(m)/sizeof(m[0]))
int id = 1;

Menu menuentries [9] = {
  { 1, 0, "Menu 1" },
  { 2, 0, "Menu 2" },
  { 3, 0, "Menu 3" },
  { 4, 0, "Menu 4" },
  { 5, 1, "Menu 1 1" },
  { 6, 1, "Menu 1 2" },
  { 7, 2, "Menu 2 1" },
  { 8, 2, "Menu 2 2" },
  { 9, 3, "Menu 3 1" }
};



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("test");

  printMenu();
  id=3;
  printMenu();
  id=5;
  printMenu();
  id=6;
  printMenu();
  id=9;
  printMenu();
}


void printMenu() {
  Serial.println("---------");
  int level=0;
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    if (menuentries[i].id == id) {
      level=menuentries[i].parent;
      break;
    }
  }

  
  for (int i = 0; i < SIZEOF(menuentries); i++) {
    if (menuentries[i].parent == level) {
      Serial.print("Name: ");
      if (menuentries[i].id == id) {
        Serial.print(">");
      }
      else {
        Serial.print(" ");
      }
      menuentries[i].execute();

    }
  }

}
void loop() {
  // put your main code here, to run repeatedly:

}
