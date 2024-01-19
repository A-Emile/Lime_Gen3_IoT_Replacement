void unlockBeeb() {
  tone(BUZZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZZER_PIN);
}

void lockBeeb() {
  tone(BUZZZER_PIN, 500, 100);
  delay(100);
  tone(BUZZZER_PIN, 400, 100);
  delay(100);
  noTone(BUZZZER_PIN);
}

void connectedBeeb() {
  tone(BUZZZER_PIN, 300, 100);
  delay(100);
  tone(BUZZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZZER_PIN);
}

void disconnectedBeeb() {
  tone(BUZZZER_PIN, 300, 100);
  delay(100);
}

void alarmBeeb() {
  if (controllerIsOn == 0) {
    digitalWrite(LOCK_PIN, HIGH);
    delay(1000);
  }
  sendControllerCommand(lightBlinkEscByte, sizeof(lightBlinkEscByte));
  for (int i = 0; i < alarm_reps; i++) {
    tone(BUZZZER_PIN, alarm_freq);
    delay(alarm_delay);
    noTone(BUZZZER_PIN);
    delay(alarm_delay);
  }
  sendControllerCommand(lightBlinkEscByte, sizeof(lightBlinkEscByte));
  if (controllerIsOn == 0) {
    digitalWrite(LOCK_PIN, LOW);
  }
}
