class MainBLECallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      String command = "";
      for (int i = 0; i < value.length(); i++) {
        command = command + value[i];
      }

      if (command == "lock") {
        lockBeeb();
        unlockForEver = 0;
        sendControllerCommand(offEscByte, sizeof(offEscByte));
        isUnlocked = 0;
        sendControllerCommand(lightOffEscByte, sizeof(lightOffEscByte));
        lightIsOn = 0;
      }
      if (command == "unlock") {
        unlockForEver = 0;
        unlockBeeb();
        if (!controllerIsOn) {
          digitalWrite(LOCK_PIN, HIGH);
          delay(3000);
          controllerIsOn = 1;
        }
        sendControllerCommand(onEscByte, sizeof(onEscByte));
        isUnlocked = 1;
        sendControllerCommand(lightOnEscByte, sizeof(lightOnEscByte));
        lightIsOn = 1;
      }
      if (command == "on") {
        digitalWrite(LOCK_PIN, HIGH);
        controllerIsOn = 1;
      }
      if (command == "off") {
        digitalWrite(LOCK_PIN, LOW);
        controllerIsOn = 0;
        isUnlocked = 1;
      }
      if (command == "unlockforever") {
        unlockForEver = 1;
        unlockScooter();
        tone(BUZZZER_PIN, 800, 100);
        delay(100);
        tone(BUZZZER_PIN, 800, 100);
        delay(100);
      }
      if (command == "alarm") {
        alarmBeeb();
      }
      if (command == "lighton") {
        sendControllerCommand(lightOnEscByte, sizeof(lightOnEscByte));
        lightIsOn = 1;
      }
      if (command == "lightoff") {
        sendControllerCommand(lightOffEscByte, sizeof(lightOffEscByte));
        lightIsOn = 0;
      }
      if (command == "shutdown") {
        digitalWrite(LOCK_PIN, LOW);
        controllerIsOn = 0;
        isUnlocked = 0;
        lightIsOn = 0;
        esp_deep_sleep_start();
      }
      if (command == "reboot") {
        digitalWrite(LOCK_PIN, LOW);
        controllerIsOn = 0;
        isUnlocked = 0;
        lightIsOn = 0;
        esp_deep_sleep_start();
      }
    }
  }
};
