void lockScooter() {
  isUnlocked = 0;
  digitalWrite(LOCK_RELAY, HIGH);
  lockBeeb();
  Serial.println("Locked");
}

void unlockScooter() {
  isUnlocked = 1;
  digitalWrite(LOCK_RELAY, LOW);
  unlockBeeb();
  Serial.println("Unlocked");
}

void turnOnLight() {
  lightIsOn = 1;
  // increase the LED brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
}

void turnOffLight() {
  lightIsOn = 0;
  // decrease the LED brightness
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
}