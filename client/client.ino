#include <SPI.h>
#include "RF24.h"                 //RF24 library

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

// RF24 setup
RF24 radio(6, 7); // Hardware Konfiguration: RF24L01 Modul
byte addresses[][6] = {"BATV10", "BARV10"}; // Device Address Transmitter/Receiver
char rxPyload[10];

void setup() {
  Serial.begin(9600);


  radio.begin();
  radio.setAutoAck(true);

  radio.setPALevel(RF24_PA_LOW); // set transmiter power
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();
  Serial.println("start");
}

void loop() {
  if ( radio.available()) {
    while (radio.available()) {
      radio.read(&rxPyload, sizeof(rxPyload) );
    }
    char mode = rxPyload[0];
  }
}
