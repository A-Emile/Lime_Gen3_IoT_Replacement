// This is the code for the original controller

unsigned long previousMillis = 0;
const long interval = 500;

void readController() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (Serial.available() == 42) {
      byte command[42];
      Serial.readBytes(command, 42);

      pDebugCharacteristic->setValue(command, sizeof(command));
      pDebugCharacteristic->notify();

      uint16_t new_checksum = crc16(command, sizeof(command) - 2, 0x1021, 0x0000, 0x0000, false, false);
      uint16_t old_checksum = (uint16_t(command[40]) << 8) | uint16_t(command[41]);  // get a pointer to the last two bytes of the command array and interpret them as a uint16_t

      // Check if the cammand has correct checksum
      if (old_checksum == new_checksum) {
        speed = (command[8] / 172.0) * max_speed;
        battery = command[19];
        throttle = command[28];
        isCharging = command[21];
        isUnlocked = command[23] == 0xF1 ? 1 : 0;
        lightIsOn = command[29] == 0x4D ? 1 : 0;
      }
    } else {
    }
    while (Serial.available() > 0) {
      char t = Serial.read();
    }
  }
}

void sendControllerCommand(byte* cmd, size_t len) {
  Serial.write(cmd, len);
}
