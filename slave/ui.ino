void functionTest() {
  switchLED('X');
  groupDisplay.setSegments(SEG_ABCD);
  counterDisplay.showNumberDec(8888, true);
  delay(400);
  switchLED('A');
  delay(500);
  switchLED('X');
  delay(400);
  switchLED('A');
  delay(500);
  switchLED('X');
  delay(400);
  groupDisplay.setSegments(SEG_AB);
}


void initUi() {
  DEBUG_PRINTLN("START");
  bool blinkMode = false;
  if (shootGroup.equals("A")) {
    groupDisplay.setSegments(SEG_AB);
  }
  else if (shootGroup.equals("C")) {
    groupDisplay.setSegments(SEG_CD);
  }
  else {
    blinkMode = true;
    groupDisplay.setSegments(SEG_AB);
  }
  if (hasPreShootTime) {
    setLight('R', blinkMode);
    counterDisplay.showNumberDec(preShootTime, false);
    startTone(NOTE_C4, 1000);
  }
  else {
    counterDisplay.showNumberDec(secondsToRun, false);
  }
}

void exitUi() {
  DEBUG_PRINTLN("STOP");
  setLight('R', true);
  startTone(NOTE_C4, 800, 400);
}

void updateUi() {
  long displayValue = secondsToRun;
  if (preShootTime != 0) {
    displayValue = preShootTime;
  }
  if (secondsToRun == yellowLEDTime) {
    setLight('Y', false);
  }


  DEBUG_PRINTLN(displayValue);
  counterDisplay.showNumberDec(displayValue, false);
}


void startTone(int freq, int duration) {
  startTone(freq, duration, 0);
}

void startTone(int freq, int duration, int repeatAfter) {
  toneStart = millis();
  toneDuration = duration;
  toneRepeatAfter = repeatAfter;
  toneFrequence = freq;
  tone(SPEAKER, toneFrequence);
}

void setLight(char code, bool doBlink) {
  DEBUG_PRINT("Ampel: ");
  DEBUG_PRINT(code);
  DEBUG_PRINT(" blink: ");
  DEBUG_PRINTLN(doBlink);
  ledCode = code;
  ledDoBlink = doBlink;
  if (doBlink) {
    ledLastBlinkTime = millis();
  }
  else {
    ledLastBlinkTime = 0;
  }
}

void switchLED(char mode) {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  ledIsOn = true;
  switch (mode) {
    case 'R':
      digitalWrite(RED_PIN, HIGH);
      break;
    case 'G':
      digitalWrite(GREEN_PIN, HIGH);
      break;
    case 'Y':
      digitalWrite(YELLOW_PIN, HIGH);
      break;
    case 'A':
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      break;
    default:
      ledIsOn = false;
      break;
  }
}
