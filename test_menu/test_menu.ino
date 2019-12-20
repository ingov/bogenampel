#include <Menu.h>
#include "SPI.h"
#include "nRF24L01.h"             //RF24 library
#include "RF24.h"                 //RF24 library
#include "OneButton.h"            //button library
#include "RotaryEncoder.h"        //rotary encoder library
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

// ***************** LCD settings *************** //
#define LCDML_DISP_cols 20
#define LCDML_DISP_rows 4
#define LCDML_DISP_cfg_cursor 0x7E   // cursor Symbol

// ***************** rotary encoder *************** //
#define ENCODER_PIN_B  A1
#define ENCODER_PIN_1  A2
#define ENCODER_PIN_2  A3

#define SIZEOF(m) (sizeof(m)/sizeof(m[0]))
int id = 1;
int directionEncoder = 0;
bool buttonFired = false;
#define BLOCKSIZE 4
#define MAXENTRIES 20

/******* Program settings *********/
int choosenProgram = 8;
int choosenTimeSetting = 13;
/******* Program settings *********/

// LCD setup
LiquidCrystal_I2C lcd(0x27, LCDML_DISP_cols, LCDML_DISP_rows);

// rotary encoder setup
RotaryEncoder encoder(ENCODER_PIN_1, ENCODER_PIN_2);

// Setup a new OneButton on pin A1.
OneButton button(ENCODER_PIN_B, true);

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

  lcd.init();
  lcd.backlight();

  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.

  // setup button
  button.attachClick(buttonClicked);
  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(80);

  delay(1000);
  updateLCD();

/*
delay(800);
  buttonfired = true; // click Program
  updateLCD();
delay(800);
  rotation = -1; // go up 
  updateLCD();
delay(800);
  buttonfired = true; // set Program
  updateLCD();
delay(800);
  rotation = 1; // go to settings
  updateLCD();
delay(800);
  buttonfired = true; // click settings
  updateLCD();
delay(800);
  buttonfired = true; // click settings
  updateLCD();
delay(800);
*/
}

void buttonClicked() {
  buttonFired = true;
}

ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
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
  button.tick();
  static int pos = 0;
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if (pos < newPos)
      directionEncoder = 1;
    else
      directionEncoder = -1;
    pos = newPos;
    updateLCD();
    directionEncoder = 0;
  }
  if (buttonFired) {
    updateLCD();
    buttonFired = false;
  }

}
