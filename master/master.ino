#include <SPI.h>
#include "nRF24L01.h"             //RF24 library
#include "RF24.h"                 //RF24 library
#include "OneButton.h"            //button library
#include "RotaryEncoder.h"        //rotary encoder library


// ***************** RF24 *************** //
#define RF24_CE_PIN 6
#define RF24_CSN_PIN 7
#define FF24_ADDRESS "00001"

// ***************** rotary encoder *************** //
#define ENCODER_PIN_B  A1
#define ENCODER_PIN_1  A2
#define ENCODER_PIN_2  A3


// radio setup
RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);                    // Hardware Konfiguration: RF24L01 Modul
const byte rf24_address[sizeof(FF24_ADDRESS)] = FF24_ADDRESS;  // set Adress for radio communication

// rotary encoder setup
RotaryEncoder encoder(ENCODER_PIN_1, ENCODER_PIN_2);

// Setup a new OneButton on pin A1.
OneButton button(ENCODER_PIN_B, true);

void setup() {

  // setup encoder
  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.

  // setup radio
  //radio.begin();
  //radio.openWritingPipe(rf24_address);
  //radio.setPALevel(RF24_PA_MIN);
  //radio.stopListening();

  // setup button
  button.attachClick(myClickFunction);
  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(80);

  Serial.begin(9600);
}

ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
}

void loop() {
  //const char text[] = "Hello World";
  //radio.write(&text, sizeof(text));

  button.tick();

  static int pos = 0;

  int newPos = encoder.getPosition();
  if (newPos < 0) {
    newPos = 0;
    encoder.setPosition(0);
  }
  if (pos != newPos) {
    Serial.print(newPos);
    Serial.println();
    pos = newPos;
  }


}

void myClickFunction() {
  Serial.println("myClickFunction");
} // myClickFunction
