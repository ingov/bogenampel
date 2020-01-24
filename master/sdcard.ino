void setupSD() {
  digitalWrite(SD_FAIL_LED, HIGH);
  digitalWrite(SD_OK_LED, HIGH);
  if (SD.begin(SD_CS_PIN)) {
    digitalWrite(SD_FAIL_LED, LOW);
  }
  else {
    digitalWrite(SD_OK_LED, LOW);
  }
}

void loadConfigFromSD() {
  setupSD();
  //long start = millis();
  String data;
  sdFileHandler = SD.open(CONFIG_FILENAME);
  if (sdFileHandler) {
    while (sdFileHandler.available()) {
      char character = sdFileHandler.read();
      data += character;
    }
    // close the file:
    sdFileHandler.close();
  }

  if (data && data.substring(0, 1).equals("P")) {
    choosenProgram = data.substring(1, 3).toInt();
    timeSetting = data.substring(4, 7).toInt();
    preShootSetting = data.substring(8, 10).toInt();
    yellowLEDSetting = data.substring(11, 13).toInt();
    choosenGroupSetting = data.substring(14, 16).toInt();
  }
  DEBUG_PRINT("got Configuration: ");
  DEBUG_PRINTLN(data);

}

void saveConfigToSD() {
  setupSD();
  //String configString = "P05T010R10Y30G19";
  String configString;

  char formatter[17];
  sprintf(formatter, "P%02dT%03dR%02dY%02dG%02d", choosenProgram, timeSetting, preShootSetting, yellowLEDSetting, choosenGroupSetting);
  configString += formatter;
  DEBUG_PRINT("save config: ");
  DEBUG_PRINTLN(formatter);

  SD.remove(CONFIG_FILENAME);
  sdFileHandler = SD.open(CONFIG_FILENAME, FILE_WRITE);
  if (sdFileHandler) {
    sdFileHandler.print(configString);
    // close the file:
    sdFileHandler.close();
  }
}
