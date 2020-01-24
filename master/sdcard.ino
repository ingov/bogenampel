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

void loadGlobalConfigFromSd() {
  setupSD();
  String data = getFileData(CONFIG_FILENAME);
  if (data && data.substring(0, 1).equals("P")) {
    choosenProgram = data.substring(1, 3).toInt();
  }
}

void loadProgramConfigFromSD() {
  setupSD();

  char filePath[9];
  sprintf(filePath, "P%02d.txt", choosenProgram);

  String data = getFileData(filePath);
  if (data && data.substring(0, 1).equals("P")) {
    choosenProgram = data.substring(1, 3).toInt();
    timeSetting = data.substring(4, 7).toInt();
    preShootSetting = data.substring(8, 10).toInt();
    yellowLEDSetting = data.substring(11, 13).toInt();
    choosenGroupSetting = data.substring(14, 16).toInt();
  }
}


String getFileData(char filePath[]) {
  String data;
  DEBUG_PRINT("read File: ");
  DEBUG_PRINTLN(filePath);

  sdFileHandler = SD.open(filePath);
  if (sdFileHandler) {
    while (sdFileHandler.available()) {
      char character = sdFileHandler.read();
      data += character;
    }
    // close the file:
    sdFileHandler.close();
  }
  DEBUG_PRINT("content is: ");
  DEBUG_PRINTLN(data);
  return data;
}

// P05T010R10Y30G19
void saveGlobalConfigToSd() {
  setupSD();
  char fileContent[3];
  sprintf(fileContent, "P%02d", choosenProgram);
  saveToFile(CONFIG_FILENAME, fileContent);
}

void saveProgramConfigToSD() {
  setupSD();

  char filePath[7];
  sprintf(filePath, "P%02d.txt", choosenProgram);

  //String configString = "P05T010R10Y30G19";
  char fileContent[17];
  sprintf(fileContent, "P%02dT%03dR%02dY%02dG%02d", choosenProgram, timeSetting, preShootSetting, yellowLEDSetting, choosenGroupSetting);
  saveToFile(filePath, fileContent);

  DEBUG_PRINT("save config: ");
  DEBUG_PRINTLN(fileContent);

}

void saveToFile(char filePath[], char fileContent[]) {

  DEBUG_PRINT("save to file: ");
  DEBUG_PRINTLN(filePath);
  DEBUG_PRINT("content: ");
  DEBUG_PRINTLN(fileContent);

  SD.remove(filePath);
  sdFileHandler = SD.open(filePath, FILE_WRITE);
  if (sdFileHandler) {
    sdFileHandler.print(fileContent);
    // close the file:
    sdFileHandler.close();
  }

}
