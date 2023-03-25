void loop() {
  readController();
  byte txByte[] = { isUnlocked, unlockForEver };
  if (deviceConnected) {
    char txString[8];
    dtostrf(txValue, 1, 2, txString);
    pSpeedCharacteristic->setValue(txString);
    pSpeedCharacteristic->notify();
    pRxCharacteristic->setValue(txByte, sizeof(txByte));
    pRxCharacteristic->notify();
    txValue++;
    delay(300);  // bluetooth stack will go into congestion, if too many packets are sent
  } else {
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    if (unlockForEver == 0) {
      lockScooter();
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

  if (isUnlocked == 1) {
    sendUnlockCommand();
    sendDisplayCommand(speed, battery, DISPLAY_STATUS_DRIVING);
  } else {
    if (deviceConnected) {
      sendDisplayCommand(speed, battery, DISPLAY_STATUS_LOCKED);
    } else {
      sendDisplayCommand(speed, battery, DISPLAY_STATUS_SCAN);
    }
  }
}