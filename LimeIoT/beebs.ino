void unlockBeeb() {
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZER_PIN);
/*
  beep(400, 100);
  beep(500, 100);
*/
}

void lockBeeb() {
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  noTone(BUZZER_PIN);
/*
  beep(500, 100);
  beep(400, 100);
*/
}

void connectedBeeb() {
  tone(BUZZER_PIN, 300, 100);
  delay(100);
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZER_PIN);
/*
  beep(300, 100);
  beep(400, 100);
  beep(500, 100);
*/
}

void disconnectedBeeb() {
  tone(BUZZER_PIN, 300, 100);
  delay(100);
//  beep(300, 100);
}

void alarmBeeb() {
  alarmIsOn = 1;
  if (!controllerIsOn) {
    turnOnController();
    delay(1500);
  }
  // avoid disorderly conduct in night mode
  if (10 > alarm_cnt) {
    playMP3("/alarm.mp3");
    sendControllerCommand(lightBlinkEscByte, sizeof(lightBlinkEscByte));
    for (int i = 0; i < alarm_reps; i++) {
      LEDmode = 0x10;
      sendDisplayLED(red, on);
//      beep(alarm_freq, alarm_delay);
      delay(alarm_delay);
      LEDmode = 0x00;
      sendDisplayLED(red, off);
      delay(alarm_delay);
    }
    sendControllerCommand(lightBlinkEscByte, sizeof(lightBlinkEscByte));
    for (int i = 0; i < alarm_reps; i++) {
      LEDmode = 0x10;
      sendDisplayLED(red, on);
      delay(alarm_delay);
      LEDmode = 0x00;
      sendDisplayLED(red, off);
      delay(alarm_delay);
    }
  }
  alarmIsOn = 0;
}
