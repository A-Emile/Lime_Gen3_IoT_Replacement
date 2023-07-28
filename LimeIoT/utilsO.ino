void lockScooter() {
  sendControllerCommand(offEscByte, sizeof(offEscByte));
  delay(100);
  isUnlocked = 0;
  sendControllerCommand(lightOffEscByte, sizeof(lightOffEscByte));
  delay(100);
  lightIsOn = 0;
}

void turnOnController() {
  digitalWrite(LOCK_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  controllerIsOn = 1;
}

void unlockScooter() {
  if (!controllerIsOn) {
    turnOnController();
    delay(1500);
  }
  sendControllerCommand(onEscByte, sizeof(onEscByte));
  delay(100);
  isUnlocked = 1;
  sendControllerCommand(lightOnEscByte, sizeof(lightOnEscByte));
  delay(100);
  lightIsOn = 1;
}

void turnOffController() {
  digitalWrite(LOCK_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  controllerIsOn = 0;
  isIdle = true;
}

// debounce GPIO input
void getPin(gpio_num_t pin, bool* state, const unsigned long dur) {
  unsigned long cur = millis() / 1000;
  static unsigned long lastRead = 0;
  static unsigned long lastHigh = 0;
  static unsigned long lastLow = 0;
  static bool high = false;
  static bool low = false;
  if (cur == lastRead) {
    return;
  }
  else {
    lastRead = cur;
  }
  if (digitalRead(pin) == HIGH) {
    if (!high) {
      lastHigh = cur;
      high = true;
    }
    if ((cur - lastHigh) > dur) {
      *state = true;
      low = false;
    }
  }
  else {
    if (!low) {
      lastLow = cur;
      low = true;
    }
    if ((cur - lastLow) > dur) {
      *state = false;
      high = false;
    }
  }
}
