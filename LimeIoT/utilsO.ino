void lockScooter() {
  isUnlocked = 0;
  digitalWrite(LOCK_PIN, LOW);
  lockBeeb();
  Serial.println("Locked");
}

void unlockScooter() {
  isUnlocked = 1;
  digitalWrite(LOCK_PIN, HIGH);
  unlockBeeb();
  Serial.println("Unlocked");
}

void turnOnLight() {
  lightIsOn = 1;
}

void turnOffLight() {
  lightIsOn = 0;
}
