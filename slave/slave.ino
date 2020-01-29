#include <SPI.h>
#include "RF24.h"                 //RF24 library
#include <TM1637Display.h>

#define NOTE_E6  1319


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

// TM1637Display settings
#define CLK 2
#define DIO_COUNTER 3
#define DIO_GROUP 4

// speaker setup
#define SPEAKER 5
// LED setup
#define RED_PIN 8
#define GREEN_PIN 9
#define YELLOW_PIN 10

// RF24 setup
RF24 radio(6, 7); // Hardware Konfiguration: RF24L01 Modul
byte addresses[][6] = {"BATV10", "BARV10"}; // Device Address Transmitter/Receiver
char rxPyload[16];

// TM1637Display constructor
TM1637Display counterDisplay(CLK, DIO_COUNTER);
TM1637Display groupDisplay(CLK, DIO_GROUP);


//***** program settings ******//
bool programStartet = false;
int secondsToRun = 0;
int preShootTime = 0;
int yellowLEDTime = 0;
long currentTs;
String shootGroup;
String nextShootGroup;
long toneStart = 0;
long toneDuration = 0;
int toneRepeatAfter = 0;
long toneRepeatStart = 0;
int toneFrequence = 0;
char ledCode = '0';
char ledLastCode = '0';
bool ledDoBlink = false;
long ledLastBlinkTime = 0;
int ledBlinkDuration = 800;
bool ledIsOn = false;
bool switchToGreen = false;
bool hasPreShootTime = true;

const uint8_t SEG_AB[] = {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
  !SEG_G, !SEG_G
};
const uint8_t SEG_CD[] = {
  !SEG_A, !SEG_A,
  SEG_A | SEG_D | SEG_E | SEG_F,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F
  
};
const uint8_t SEG_ABCD[] = {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F,
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F
};


void setup() {
  DEBUG_BEGIN(9600);
  // setup LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  // setup radio
  radio.begin();
  radio.setAutoAck(true);
  radio.setPALevel(RF24_PA_LOW); // set transmiter power
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();

  // setup 7 seg Display
  counterDisplay.setBrightness(0x0f);
  groupDisplay.setBrightness(0x0f);
  

  DEBUG_PRINTLN("Client Up");
  functionTest();
  initUi();
}

void loop() {
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&rxPyload, sizeof(rxPyload) );
    }
    DEBUG_PRINT("got Payload: ");
    DEBUG_PRINTLN(rxPyload);
    String data = String(rxPyload);
    shootGroup = data.substring(0, 1);
    nextShootGroup = data.substring(1, 2);
    secondsToRun = String(data.substring(2, 5)).toInt();
    preShootTime = String(data.substring(5, 7)).toInt();
    yellowLEDTime = String(data.substring(7, 9)).toInt();
    programStartet = true;
    hasPreShootTime = preShootTime > 0;
    currentTs = millis();
    initUi();
    switchToGreen = true;
  }

  if (programStartet) {
    long now = millis();
    if (preShootTime > 0) {
      if (now - currentTs >= 1000) {
        preShootTime--;
        updateUi();
        currentTs = millis();
      }
    }
    else {
      if (switchToGreen) {
        setLight('G', false);
        startTone(NOTE_E6, 500);
        switchToGreen = false;
      }
      if (secondsToRun == 0) {
        programStartet = false;

        exitUi();

      }
      else {
        if (now - currentTs >= 1000) {
          secondsToRun--;
          updateUi();
          currentTs = millis();
        }
      }
    }
  }

  //manage Speaker
  if (toneStart != 0 && toneDuration != 0 && toneRepeatStart == 0) {
    long now = millis();
    if (toneStart + toneDuration < now) {
      noTone(SPEAKER);
      if (toneRepeatAfter == 0) {
        toneStart = 0;
        toneDuration = 0;
        toneFrequence = 0;
      }
      else {
        toneRepeatStart = millis();
      }
    }
  }
  if (toneRepeatAfter != 0 && toneRepeatStart != 0) {
    long now = millis();
    if (toneRepeatStart + toneRepeatAfter < now) {
      toneRepeatStart = 0;
      startTone(toneFrequence, toneDuration);
    }
  }
  // manage LED
  if (ledCode != '0' || ledDoBlink) {
    if (ledCode != '0') {
      switchLED(ledCode);
      ledLastCode = ledCode;
      ledCode = '0';
    }
    if (ledDoBlink) {
      long now = millis();
      if (ledLastBlinkTime + ledBlinkDuration < now) {
        char code = ledIsOn ? '0' : ledLastCode;
        switchLED(code);
        ledLastBlinkTime = millis();
      }
    }
  }
}
