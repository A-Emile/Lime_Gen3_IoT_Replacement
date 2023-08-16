unsigned long prevMillis = 0;
const long linterval = 250;

unsigned long lastOnTime = 0;
const unsigned short sleepTimer = 3 * 60 * 60; // 3 hours

// inhibit driving without hearthbeat timeout
unsigned long lastConnected = 0;
const unsigned short lockTimer = 3 * 60 ; // 3 minutes

void loop() {
  unsigned long currentTime = millis() / 1000;
/*
  // If more than 3 hours have passed, go to deep sleep
  if ((currentTime - lastOnTime) > sleepTimer) {
    lockScooter();
    turnOffController();
    digitalWrite(DISPLAY_PIN, HIGH);
    // wait for BOOT_PIN state was updated
    if (!isBooted || (currentTime - lastOnTime) > sleepTimer + 30) {
      lastOnTime = currentTime;
      esp_deep_sleep_start();
    }
  }
*/
  if (isUnlocked || isCharging) {
    lastOnTime = currentTime;
  }
  // arm the alarm on shock sensor
  if (isDisconnected && (currentTime - lastConnected) > lockTimer && (currentTime - lastOnTime) > lockTimer) {
    isDisconnected = false;
  }
/*
  // wake on shock sensor
  if (digitalRead(SHOCK_PIN) == HIGH && !alarmIsOn && !deviceConnected && !isDisconnected && !isUnlocked && !unlockForEver) {
    digitalWrite(DISPLAY_PIN, LOW);
    alarmBeeb();
    alarm_cnt++; // avoid disorderly conduct in night mode
    lastOnTime = millis() / 1000;
    isIdle = false;
  }
*/
  // wake on charger (decrease idle time with pull-down resistor)
  getPin(BOOT_PIN, &isBooted, 5);
  if (isBooted || (currentTime % 80000 == 0)) {
    if (!controllerIsOn && !isIdle) { // update battery once a day
      digitalWrite(DISPLAY_PIN, LOW);
      lastOnTime = currentTime;
      turnOnController();
    }
  }
  // lock on charging
  if (isUnlocked && isCharging && !unlockForEver && !commandIsSending && !alarmIsOn && !isMP3Playing) {
    playMP3("/lock.mp3");
    lockScooter();
  }
  // turn off on charging idle
  if (!isIdle && controllerIsOn && !alarmIsOn && !deviceConnected && !unlockForEver && (currentTime - lastOnTime) > 30) {
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
    } else if (isUnlocked && !unlockForEver && (currentTime - lastConnected > lockTimer)) {
        lockScooter();
      }
    if (controllerIsOn && !commandIsSending) {
      if (deviceConnected || unlockForEver) {
        sendControllerCommand(hearthBeatEscByte, sizeof(hearthBeatEscByte));
        lastConnected = currentTime;
      }
    }
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    if (!unlockForEver && !isCharging) {
      turnOffController();
    }
    playMP3("/disconnected.mp3");
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
    isDisconnected = true;
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
