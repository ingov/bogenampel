#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>

/* configure 7 Seg display */
#define CLK 2
#define DIO 3

const uint8_t INIT_SEG[] = {SEG_G,SEG_G,SEG_G,SEG_G};


TM1637Display display(CLK, DIO);


void setup(){
  // setup 7 seg display
  display.setBrightness(0x0f);
  display.setSegments(INIT_SEG);
}

void loop() {



    delay(10000);

}
