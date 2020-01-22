
void loadConfigFromSlave() {
  //long start = millis();
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
    yellowLEDSetting = data.substring(11, 13).toInt();
    choosenGroupSetting = data.substring(14, 16).toInt();
  }
  DEBUG_PRINT("got Configuration: ");
  DEBUG_PRINTLN(data);

}

void saveConfigToSlave() {

  //String configString = "P05T010R10Y30G19";
  String configString;

  char formatter[17];
  sprintf(formatter, "P%02dT%03dR%02dY%02dG%02d", choosenProgram, timeSetting, preShootSetting, yellowLEDSetting, choosenGroupSetting);
  configString += formatter;
  DEBUG_PRINT("save config: ");
  DEBUG_PRINTLN(formatter);

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(configString.c_str());
  Wire.endTransmission();

}
