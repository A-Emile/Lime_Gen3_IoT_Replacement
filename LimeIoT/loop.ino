unsigned long prevMillis = 0;
const long linterval = 250;

unsigned long lastOnTime = 0;
const long sleepTimer = 3600; // 1 hour

void loop() {
  unsigned long currentTime = millis() / 1000;

  // If more than 1 hours have passed, go to deep sleep
  if ((currentTime - lastOnTime) > sleepTimer) {
    lockScooter();
    turnOffController();
//    digitalWrite(DISPLAY_PIN, HIGH);
    // wait for BOOT_PIN state was updated
    if (!isBooted) {
      lastOnTime = currentTime;
//      esp_deep_sleep_start();
    }
  }
  else if (isUnlocked || isCharging) {
    lastOnTime = currentTime;
  }

  // wake on shock sensor
  if (digitalRead(SHOCK_PIN) == HIGH && !alarmIsOn && !isUnlocked && !unlockForEver) {
    digitalWrite(DISPLAY_PIN, LOW);
    alarmBeeb();
  }

  // wake on charger (decrease idle time with pull-down resistor)
  getPin(BOOT_PIN, &isBooted, 5);
  if (isBooted && !controllerIsOn && !isIdle) {
    digitalWrite(DISPLAY_PIN, LOW);
    lastOnTime = currentTime;
    turnOnController();
    lockScooter();
  }
  // turn off on charging idle
  if (!isIdle && controllerIsOn && !alarmIsOn && !deviceConnected && (currentTime - lastOnTime) > 5) {
    lockScooter();
    turnOffController();
  }
  if (isIdle && !isBooted) {
    isIdle = false;
  }

  unsigned long currMillis = millis();
  byte txByte[] = { isUnlocked, unlockForEver, speed, battery, throttle, lightIsOn, controllerIsOn, isCharging, alarmIsOn };
  byte settingsByte[] = { max_speed, alarm_delay, alarm_freq, alarm_reps };

  if (currMillis - prevMillis >= linterval) {
    prevMillis = currMillis;  // update prevMillis with current time

    if (deviceConnected) {
      pSettingsCharacteristic->setValue(settingsByte, sizeof(txByte));
      pMainCharacteristic->setValue(txByte, sizeof(txByte));
      pMainCharacteristic->notify();
    }
    if (controllerIsOn) {
      if (!commandIsSending) {
        sendControllerCommand(hearthBeatEscByte, sizeof(hearthBeatEscByte));
      }
    }
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    if (unlockForEver == 0) {
      turnOffController();
    }
    playMP3("/disconnected.mp3");
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    unlockForEver = 0;
    Serial.println("connecting");
    oldDeviceConnected = deviceConnected;
  }
  if (controllerIsOn || isUnlocked) {
    readController();
  }
}
