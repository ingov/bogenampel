#include <Menu.h>
#include "SPI.h"
#include "nRF24L01.h"             //RF24 library
#include "RF24.h"                 //RF24 library
#include "OneButton.h"            //button library
#include "RotaryEncoder.h"        //rotary encoder library
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

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
#define MAXENTRIES 18

/******* Program settings *********/
int choosenProgram = 8;
int choosenTimeSetting = 13;
int choosenGroupSetting = 18;
bool programStartet = false;
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
  { 4, 0, startSession, true, "Program starten" },
  { 5, 2, setProgram, true, "Program 1" },
  { 6, 2, setProgram, true, "Program 2" },
  { 7, 2, setProgram, true, "Program 3" },
  { 8, 2, setProgram, true, "Program 4" },
  { 9, 2, setProgram, true, "Program 5" },
  { 10, 3, setTime, true, "Zeiteinstellung" },
  { 11, 3, setGroup, true, "Gruppen" },
  { 12, 3, goBack, true, "zuruek" },
  { 13, 10, setTime, true, "60" },
  { 14, 10, setTime, true, "120" },
  { 15, 10, setTime, true, "180" },
  { 16, 10, setTime, true, "240" },
  { 17, 11, setGroup, true, "AB" },
  { 18, 11, setGroup, true, "AB/CD" }
};

byte upArrow[] = {B00100, B01110, B10101, B00100, B00100, B00100, B00100, B00100};

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.createChar(1, upArrow);


  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.

  // setup button
  button.attachClick(buttonClicked);
  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(80);

  delay(1000);
  drawMenu();

}

ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
}



void loop() {
  button.tick();
  static int pos = 0;
  int newPos = encoder.getPosition();

  // programm loop
  if (programStartet) {
    lcd.clear();
    Serial.println(menuentries[choosenProgram - 1].lable);
    Serial.println(menuentries[choosenGroupSetting - 1].lable);
    Serial.println(menuentries[choosenTimeSetting - 1].lable);

    lcd.print(menuentries[choosenProgram - 1].lable);
    lcd.print(" gestartet");
    lcd.setCursor(0, 1);
    lcd.print(menuentries[choosenGroupSetting - 1].lable);
    lcd.print(" ");
    lcd.print(menuentries[choosenTimeSetting - 1].lable);
    lcd.setCursor(0, 2);
    lcd.print("Weiter  Pause  Stop");

    int chooser = -1;
    bool stateChange = false;

    while (programStartet) {
      button.tick();
      static int pos = 0;
      int newPos = encoder.getPosition();
      if (chooser == -1) {
        chooser = 0;
        pos = newPos;
        stateChange = true;
      }
      if (pos != newPos) {
        if (pos < newPos) {
          chooser += 1;
          stateChange = true;
        }
        else {
          chooser -= 1;
          stateChange = true;
        }
        pos = newPos;
      }
      chooser = (chooser < 0) ? 0 : chooser;
      chooser = (chooser > 2) ? 2 : chooser;
      if (stateChange) {
        lcd.setCursor(0, 3);
        lcd.print("   ");
        for (int i = 0; i < chooser; i++) {
          lcd.print("       ");
        }
        lcd.printByte(1);
        for (int i = 2; i > chooser; i--) {
          lcd.print("       ");
        }
        stateChange = false;
      }

      if (buttonFired) {
        lcd.setCursor(0, 3);
        lcd.print("send stop signal...");
        lcd.print(chooser);
        //noInterrupts();
        delay(2000);
        //interrupts();
        
        buttonFired = false;
        programStartet = false;
        id = 4;
        drawMenu();
      }
    }
  }
  else if (pos != newPos) {
    if (pos < newPos)
      directionEncoder = 1;
    else
      directionEncoder = -1;
    pos = newPos;
    drawMenu();
    directionEncoder = 0;
  }
  if (buttonFired) {
    drawMenu();
    buttonFired = false;
  }

}
