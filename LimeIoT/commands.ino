class RxCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      String command = "";
      for (int i = 0; i < value.length(); i++) {
        command = command + value[i];
      }

      Serial.println("*********");
      Serial.print("command = ");
      Serial.println(command);  // Presenta valor.

      if (command == "lock") {
        unlockForEver = 0;
        lockScooter();
      }
      if (command == "unlock") {
        unlockForEver = 0;
        unlockScooter();
      }
      if (command == "unlockforever") {
        unlockForEver = 1;
        unlockScooter();
        tone(BUZZZER_PIN, 800, 100);
        delay(100);
        tone(BUZZZER_PIN, 800, 100);
        delay(100);
      }
      if (command == "light") {
        tone(BUZZZER_PIN, 800, 100);
        if (lightIsOn == 0) {
          turnOnLight();
        } else {
          turnOffLight();
        }
      }
      if (command == "alarm") {
        for (int i = 0; i < 60; i++) {
          // Turn the speaker on at a frequency of 1000 Hz
          tone(BUZZZER_PIN, 3000);

          // Wait for 0.1 seconds (the beep duration)
          delay(200);

          // Turn the speaker off
          noTone(BUZZZER_PIN);

          // Wait for 0.1 seconds (the beep interval)
          delay(200);
        }
      }
    }
  }
};