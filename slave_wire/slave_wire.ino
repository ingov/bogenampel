#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// i2c address for master-slave communication
#define SLAVE_ADDRESS 0x04
#define CONFIG_SIZE 16

#define SD_CS_PIN 4
#define CONFIG_FILENAME "config.txt"
#define FAIL_LED 5
#define OK_LED 6

File myFile;


void setup() {

  pinMode (FAIL_LED, OUTPUT);
  pinMode (OK_LED, OUTPUT);
  digitalWrite(FAIL_LED, HIGH);
  digitalWrite(OK_LED, HIGH);

  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  if (SD.begin(SD_CS_PIN)) {
    digitalWrite(FAIL_LED, LOW);
  }
  else {
    digitalWrite(OK_LED, LOW);
  }


  Serial.println("Ready!");
}

void receiveData(int byteCount) {
  String data;
  while (Wire.available()) {
    char foo = Wire.read();
    data += foo;
  }
  // save Data on SD-Card
  sdPersist(data);

}

void sendData() {
  String configString = sdLoad();
  char data[CONFIG_SIZE];
  configString.toCharArray(data, CONFIG_SIZE + 1);
  Wire.write(data);
}

void loop() {
}

void sdPersist(String data) {
  SD.remove(CONFIG_FILENAME);
  myFile = SD.open(CONFIG_FILENAME, FILE_WRITE);
  if (myFile) {
    myFile.print(data);
    // close the file:
    myFile.close();
  }
}

String sdLoad() {
  String data;
  myFile = SD.open(CONFIG_FILENAME);
  if (myFile) {
    while (myFile.available()) {
      char character = myFile.read();
      data += character;
    }
    // close the file:
    myFile.close();
  }
  return data;
}
