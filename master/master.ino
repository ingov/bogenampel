#include <Menu.h>
#include <SPI.h>
#include <nRF24L01.h>             //RF24 library
#include <RF24.h>                 //RF24 library
#include <OneButton.h>            //button library
#include <RotaryEncoder.h>        //rotary encoder library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>


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
#define ENCODER_PIN_1  A8
#define ENCODER_PIN_2  A9
/******* menu config BLOCKSIZE is number of rows on lcd MAXENTRIES length of menu ********/
#define BLOCKSIZE 4
#define MAXENTRIES 20
/*********** SD Card settings *********/
#define SD_CS_PIN 4
#define CONFIG_SIZE 16
const char CONFIG_FILENAME[10] = "config.txt";
#define SD_FAIL_LED 2
#define SD_OK_LED 3

int id = 1;
int directionEncoder = 0;
bool buttonFired = false;

/******* Program settings *********/
int choosenProgram = 5;
int choosenTimeSetting = 17;
int choosenPreShootSetting = 16;
int choosenYellowLEDSetting = 18;
int timeSetting = 10;
int timeSettingMax = 120;
const int timeSettingStep = 10;
int preShootSetting = 10;
int preShootSettingMax = 10;
const int preShootSettingStep = 1;
int yellowLEDSetting = 30;
const int yellowLEDSettingMax = 60;
int yellowLEDSettingStep = 5;
int choosenGroupSetting = 19;
bool programStartet = false;
int programLaps = 0;
int programLoops = 0;
int programGroup = 0;
int nextGroup = 0;
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

// SD-Card
File sdFileHandler;


// represents the Menu-structure
const Menu menuentries [MAXENTRIES] = {
  { 1, 0, noFunc, noFunc, false, false, "Bogenampel V1.0"},
  { 2, 0, selectProgram, noFunc, true, false, "Program" },
  { 3, 0, noFunc, noFunc, true, false, "Einstellungen" },
  { 4, 0, startSession, noFunc, true, false, "Program starten" },
  { 5, 2, setProgram, noFunc, true, false, "WA 90 70 60" },
  { 6, 2, setProgram, noFunc, true, false, "WA 50 30" },
  { 7, 2, setProgram, noFunc, true, false, "WA Halle" },
  { 8, 2, setProgram, noFunc, true, false, "Finale" },
  { 9, 2, setProgram, noFunc, true, false, "Frei" },
  { 10, 3, setPreShootTime, preShotTrigger, true, false, "Vorbereitung" },
  { 11, 3, setTime, timeTrigger, true, false, "Gr\xF5""nphase" },
  { 12, 3, setYellowLEDTime, yellowLEDTrigger, true, false, "Gelbphase" },
  { 13, 3, setGroup, noFunc, true, false, "Gruppen" },
  { 14, 3, saveSettings, noFunc, true, false, "Speichern" },
  { 15, 3, goBack, noFunc, true, false, "zur\xF5""ck" },
  { 16, 10, setPreShootTime, preShotTrigger, true,  true, "Vorbereitung"},
  { 17, 11, setTime, timeTrigger, true,  true, "Zeit pro Passe"},
  { 18, 12, setYellowLEDTime, yellowLEDTrigger, true,  true, "Gelbphase"},
  { 19, 13, setGroup, noFunc, true, false, "AB" },
  { 20, 13, setGroup, noFunc, true, false, "AB/CD" }
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

  //sd card init
  pinMode (SD_FAIL_LED, OUTPUT);
  pinMode (SD_OK_LED, OUTPUT);
  digitalWrite(SD_FAIL_LED, HIGH);
  digitalWrite(SD_OK_LED, HIGH);
  // first SD call
  loadGlobalConfigFromSd();
  loadProgramConfigFromSD();


  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCMSK2 |= (1 << PCINT16) | (1 << PCINT17);  // This enables the interrupt for pin A8 and A9 of Port C.
  PCICR |= (1 << PCIE2);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.

  // setup button
  button.attachClick(buttonClicked);
  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(80);
  drawMenu();
}

ISR (PCINT2_vect) {
  encoder.tick(); // just call tick() to check the state.
}

void loop() {
  button.tick();
  static int pos = 0;
  int newPos = encoder.getPosition();

  // programm loop
  if (programStartet) {
    lcd.clear();

    lcd.print(menuentries[choosenProgram - 1].lable);
    lcd.print(" ");
    lcd.print(menuentries[choosenGroupSetting - 1].lable);
    lcd.setCursor(0, 1);
    lcd.print("Zeit ");
    lcd.print(timeSetting);
    lcd.setCursor(9, 1);
    lcd.print("AB");
    lcd.setCursor(12, 1);
    lcd.print("Runde 1");

    lcd.setCursor(0, 2);
    lcd.print("Weiter  Pause  Stop");
    int chooser = -1;
    bool stateChange = false;
    programLoops++;

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

          calcNextLoopParams();
          if (sendToClinet(chooser)) {
            lcd.setCursor(9, 1);
            lcd.print((groups[programGroup] == 'A') ? "AB" : "CD");
            lcd.setCursor(12, 1);
            lcd.print("Runde ");
            lcd.print(programLaps);
            programLoops++;
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
          programGroup = 0;
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


void calcNextLoopParams() {
  if (choosenGroupSetting == 20) {
    programLaps += (programLoops + 1) % 2;
    int tmpgrp = (programLoops % 2);
    nextGroup = programLaps % 2;
    programGroup = (nextGroup == tmpgrp) ? 1 : 0;
  }
  else {
    programLaps += 1;
    programGroup = 0;
    nextGroup = 0;
  }

}
