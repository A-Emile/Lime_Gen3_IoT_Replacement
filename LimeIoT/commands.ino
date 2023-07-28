class MainBLECallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      String command = "";
      for (int i = 0; i < value.length(); i++) {
        command = command + value[i];
      }
      commandIsSending = true;
      if (command == "lock") {
        playMP3("/lock.mp3");
        delay(100);
        unlockForEver = 0;
        lockScooter();
      }
      if (command == "unlock") {
        playMP3("/unlock.mp3");
        delay(100);
        unlockForEver = 0;
        unlockScooter();
      }
      if (command == "on") {
        turnOnController();
      }
      if (command == "off") {
        lockScooter();
        turnOffController();
      }
      if (command == "unlockforever") {
        playMP3("/unlock.mp3");
        delay(100);
        unlockForEver = 1;
        unlockScooter();
      }
      if (command == "alarm") {
        alarmBeeb();
      }
      if (command == "lighton") {
        sendControllerCommand(lightOnEscByte, sizeof(lightOnEscByte));
        delay(100);
        lightIsOn = 1;
      }
      if (command == "lightoff") {
        sendControllerCommand(lightOffEscByte, sizeof(lightOffEscByte));
        delay(100);
        lightIsOn = 0;
      }
      if (command == "shutdown") {
        lockScooter();
        turnOffController();
        esp_deep_sleep_start();
      }
      if (command == "reboot") {
        lockScooter();
        turnOffController();
        esp_deep_sleep_start();
      }
      commandIsSending = false;
    }
  }
};
