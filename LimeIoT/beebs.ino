void unlockBeeb() {
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZER_PIN);
}

void lockBeeb() {
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  noTone(BUZZER_PIN);
}

void connectedBeeb() {
  tone(BUZZER_PIN, 300, 100);
  delay(100);
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZER_PIN);
}

void disconnectedBeeb() {
  tone(BUZZER_PIN, 300, 100);
  delay(100);
}

void alarmBeeb() {
  alarmIsOn = 1;
  if (!controllerIsOn) {
    turnOnController();
    delay(1500);
  }
  playMP3("/alarm.mp3");
  sendControllerCommand(lightBlinkEscByte, sizeof(lightBlinkEscByte));
  for (int i = 0; i < alarm_reps; i++) {
    sendDisplayLED(red, on);
    delay(alarm_delay);
    sendDisplayLED(red, off);
    delay(alarm_delay);
  }
  delay(500);
  sendControllerCommand(lightBlinkEscByte, sizeof(lightBlinkEscByte));
  for (int i = 0; i < alarm_reps; i++) {
    sendDisplayLED(red, on);
    delay(alarm_delay);
    sendDisplayLED(red, off);
    delay(alarm_delay);
  }
  alarmIsOn = 0;
}
