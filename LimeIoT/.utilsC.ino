
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