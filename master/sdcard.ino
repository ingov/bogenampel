
void loadConfigFromSlave() {
  long start = millis();
  String data;

  Wire.requestFrom(SLAVE_ADDRESS, CONFIG_SIZE);
  while (Wire.available())  {
    char character = Wire.read();
    data += character;
  }
  if (data && data.substring(0, 1).equals("P")) {
    choosenProgram = data.substring(1, 3).toInt();
    timeSetting = data.substring(4, 7).toInt();
    preShootSetting = data.substring(8, 10).toInt();
  }
  DEBUG_PRINT("got Configuration: ");
  DEBUG_PRINT(data);
  DEBUG_PRINT(" ms: ");
  DEBUG_PRINTLN(millis() - start);
}

void saveConfigToSlave() {

  //String configString = "P01T050R05";
  String configString;

  char formatter[11];
  sprintf(formatter, "P%02dT%03dR%02d", choosenProgram, timeSetting, preShootSetting);
  configString += formatter;
  DEBUG_PRINT("save config: ");
  DEBUG_PRINTLN(configString);

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(configString.c_str());
  Wire.endTransmission();

}
