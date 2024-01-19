
unsigned long prevMillis = 0;
const long linterval = 250;


void loop() {
  unsigned long currentTime = millis() / 1000;

  // Calculate the time difference in seconds
  unsigned long timeDifference = 1 * 60 * 60 - currentTime;  // 2 hours

 if (timeDifference <= 0) {
    // If more than 1 hour have passed, go to deep sleep
    digitalWrite(LOCK_PIN, LOW);
    controllerIsOn = 0;
    //esp_deep_sleep_start();
  }
  
  if (digitalRead(SHOCK_PIN) == LOW && !isUnlocked) {
    alarmBeeb();
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
      lockScooter();
      digitalWrite(LOCK_PIN, LOW);
      controllerIsOn = 0;
    }
    disconnectedBeeb();
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
  if (controllerIsOn) {
    readController();
  }
}
