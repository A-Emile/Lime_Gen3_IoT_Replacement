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