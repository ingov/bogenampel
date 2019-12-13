
#include "OneButton.h"

OneButton button(A1, true);


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);      // sets the digital pin as output
  button.attachClick(myClickFunction);

  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(50);
  // You may have to modify the next 2 lines if using another pin than A1
  PCICR |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT9);  // This enables the interrupt for pin 1 of Port C: This is A1.

}

// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A1: exactly where we need to check.
ISR(PCINT1_vect) {
  // keep watching the push button:
  Serial.println("tick");
  button.tick(); // just call tick() to check the state.
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

void myClickFunction() {
  Serial.println("click");
  static int m = LOW;
  // reverse the LED 
  m = !m;
  digitalWrite(13, m);
} // doubleclick
