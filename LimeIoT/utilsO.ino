void lockScooter() {
  isUnlocked = 0;
  lockBeeb();
}

void unlockScooter() {
  isUnlocked = 1;
  digitalWrite(LOCK_PIN, HIGH);
  unlockBeeb();
}

void turnOnController() {
  controllerIsOn = 1;
  digitalWrite(LOCK_PIN, HIGH);
}

void turnOffController() {
  controllerIsOn = 0;
  digitalWrite(LOCK_PIN, LOW);
}
