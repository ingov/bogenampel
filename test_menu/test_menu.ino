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
/* configure debugging via serial.print */
#define DEBUG
#ifdef DEBUG
#define DEBUG_BEGIN(x) Serial.begin(x);
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_BEGIN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif
/* helper for "real" sizeof */
#define SIZEOF(m) (sizeof(m)/sizeof(m[0]))



// ***************** LCD settings *************** //
#define LCDML_DISP_cols 20
#define LCDML_DISP_rows 4
#define LCDML_DISP_cfg_cursor 0x7E   // cursor Symbol
// ***************** rotary encoder *************** //
#define ENCODER_PIN_B  A1
#define ENCODER_PIN_1  A2
#define ENCODER_PIN_2  A3
/******* menu config BLOCKSIZE is number of rows on lcd MAXENTRIES length of menu ********/
#define BLOCKSIZE 4
#define MAXENTRIES 17

const int chipSelect = 4;
// set up variables using the SD utility library functions:
//Sd2Card card;
//SdVolume volume;
//SdFile root;


int id = 1;
int directionEncoder = 0;
bool buttonFired = false;

/******* Program settings *********/
int choosenProgram = 5;
int choosenTimeSetting = 14;
int choosenPreShootSetting = 15;
int timeSetting = 10; // test for time
int timeSettingMax = 120;
int timeSettingStep = 10;
int preShootSetting = 10; // test for time
int preShootSettingMax = 10;
int preShootSettingStep = 1;
int choosenGroupSetting = 17;
bool programStartet = false;
int programLaps = 0;
int groupCounter = 0;
/******* Program settings *********/

// RF24 setup
RF24 radio(6, 7); // Hardware Konfiguration: RF24L01 Modul
byte addresses[][6] = {"BATV10", "BARV10"}; // Device Address Transmitter/Receiver

// LCD setup
LiquidCrystal_I2C lcd(0x27, LCDML_DISP_cols, LCDML_DISP_rows);

// rotary encoder setup
RotaryEncoder encoder(ENCODER_PIN_1, ENCODER_PIN_2);

// Setup a new OneButton on pin A1.
OneButton button(ENCODER_PIN_B, true);


// represents the Menu-structure
Menu menuentries [MAXENTRIES] = {
  { 1, 0, noFunc, noFunc, false, false, "Hauptmenu"},
  { 2, 0, selectProgram, noFunc, true, false, "Program" },
  { 3, 0, noFunc, noFunc, true, false, "Program Einstellung" },
  { 4, 0, startSession, noFunc, true, false, "Program starten" },
  { 5, 2, setProgram, noFunc, true, false, "Program 1" },
  { 6, 2, setProgram, noFunc, true, false, "Program 2" },
  { 7, 2, setProgram, noFunc, true, false, "Program 3" },
  { 8, 2, setProgram, noFunc, true, false, "Program 4" },
  { 9, 2, setProgram, noFunc, true, false, "Program 5" },
  { 10, 3, setTime, timeTrigger, true, false, "Zeiteinstellung" },
  { 11, 3, setPreShootTime, preShotTrigger, true, false, "Rotphase" },
  { 12, 3, setGroup, noFunc, true, false, "Gruppen" },
  { 13, 3, goBack, noFunc, true, false, "zuruek" },
  { 14, 10, setTime, timeTrigger, true,  true, "Zeit pro Gruppe"},
  { 15, 11, setPreShootTime, preShotTrigger, true,  true, "Zeit vor Start"},
  { 16, 12, setGroup, noFunc, true, false, "AB" },
  { 17, 12, setGroup, noFunc, true, false, "AB/CD" }
};
char groups[2] = "AC";


/* special character up Arrow */
byte upArrow[] = {B00100, B01110, B10101, B00100, B00100, B00100, B00100, B00000};
void setup() {
  DEBUG_BEGIN(9600);


  
  // init RF24
  radio.begin();
  radio.setAutoAck(true);
  radio.setPALevel(RF24_PA_LOW); // set transmiter power
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.stopListening();


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
    DEBUG_PRINTLN(menuentries[choosenProgram - 1].lable);
    DEBUG_PRINTLN(menuentries[choosenGroupSetting - 1].lable);
    DEBUG_PRINTLN(timeSetting);

    lcd.print(menuentries[choosenProgram - 1].lable);
    lcd.print(" ");
    lcd.print(menuentries[choosenGroupSetting - 1].lable);
    lcd.setCursor(0, 1);
    lcd.print("Zeit ");
    lcd.print(timeSetting);
    lcd.setCursor(9, 1);
    lcd.print((groups[(programLaps + groupCounter % 2) % 2] == 'A') ? "AB" : "CD" );
    lcd.setCursor(12, 1);
    lcd.print("Runde ");
    lcd.print(programLaps + 1);

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
        buttonFired = false;

        if (chooser == 0) {
          // toggle groups (ab/cd/cd/ab/ab/cd...)
          if (programLaps % 2) {
            groupCounter++;
          }
          programLaps++;
          if (sendToClinet(chooser)) {
            lcd.setCursor(9, 1);
            lcd.print((groups[(programLaps + groupCounter % 2) % 2] == 'A') ? "AB" : "CD");
            lcd.setCursor(12, 1);
            lcd.print("Runde ");
            lcd.print(programLaps + 1);
          }
          else {
            programLaps--;
          }
        }
        else {
          sendToClinet(chooser);
          lcd.setCursor(0, 3);
          lcd.print("send stop signal...");
          lcd.print(chooser);
          //noInterrupts();
          delay(800);
          //interrupts();

          programStartet = false;
          programLaps = 0;
          groupCounter = 0;
          id = 4;
          drawMenu();

        }
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
