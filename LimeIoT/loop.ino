
unsigned long prevMillis = 0;
const long linterval = 250;


void loop() {
  unsigned long currMillis = millis();
  byte txByte[] = { isUnlocked, unlockForEver, speed, battery, lightIsOn };
  byte settingsByte[] = { max_speed, alarm_delay, alarm_freq, alarm_reps };

  if (currMillis - prevMillis >= linterval) {
    prevMillis = currMillis;  // update prevMillis with current time

    if (deviceConnected) {
      pSettingsCharacteristic->setValue(settingsByte, sizeof(txByte));
      pMainCharacteristic->setValue(txByte, sizeof(txByte));
      pMainCharacteristic->notify();
    }
    if (isUnlocked == 1) {
      sendControllerCommand(hearthBeatEscByte, sizeof(hearthBeatEscByte));
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