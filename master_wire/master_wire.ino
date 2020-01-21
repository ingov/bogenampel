#include <Wire.h>

// i2c address for master-slave communication
#define SLAVE_ADDRESS 0x04
#define CONFIG_SIZE 10

void setup() {
  Wire.begin();

  //Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  Serial.println("Ready!");

}


void loop() {
  long start = millis();
  Wire.beginTransmission(4); // transmit to device #4
  String configString = "P01T050R05";

  char data[CONFIG_SIZE];
  configString.toCharArray(data, CONFIG_SIZE+1);
  Wire.write(data);
  Wire.endTransmission();    // stop transmitting
  Serial.print("send Config in ms: ");
  Serial.println(millis() - start);

  delay(2000);

  loadFromSlave();
  delay(2000);

}

void loadFromSlave() {
  long start = millis();
  String data;

  Wire.requestFrom(SLAVE_ADDRESS, CONFIG_SIZE);
  while (Wire.available())  {
    char character = Wire.read();
    data += character;
  }
  Serial.print("got Configuration: ");
  Serial.print(data);
  Serial.print(" ms: ");
  Serial.println(millis() - start);

}
