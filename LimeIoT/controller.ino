// This is the code for the original controller

unsigned long previousMillis = 0;
const long interval = 500;

void readController() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (Serial1.available() == 42) {
      byte command[42];
      Serial1.readBytes(command, 42);

      pDebugCharacteristic->setValue(command, sizeof(command));
      pDebugCharacteristic->notify();

      uint16_t new_checksum = crc16(command, sizeof(command) - 2, 0x1021, 0x0000, 0x0000, false, false);
      uint16_t old_checksum = (uint16_t(command[40]) << 8) | uint16_t(command[41]);  // get a pointer to the last two bytes of the command array and interpret them as a uint16_t

      // Check if the command has correct checksum
      if (old_checksum == new_checksum) {
        speed = (command[8] / 172.0) * max_speed;
        battery = command[19];
        throttle = command[28];
        isCharging = command[21];
        isUnlocked = command[23] == 0xF1 ? 1 : 0;
        lightIsOn = command[29] == 0x4D ? 1 : 0;
        lastBattery = battery;
      }
    }
    while (Serial1.available() > 0) {
      char t = Serial1.read();
    }
  }
}

void sendControllerCommand(byte* cmd, size_t len) {
  static bool isSending = 0;
  while (isSending) {
    delay(100);
  }
  isSending = 1;
  Serial1.write(cmd, len);
  delay(500);
  isSending = 0;
}
