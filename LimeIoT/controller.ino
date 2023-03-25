// This is the code for the original controller

  void readController() {
    byte command[42];
    Serial.readBytes(command, 42);
    if (command[0] == 0x46) {
      speed = (command[8] / 172.0) * 27.2;
      battery = command[19];
    }
  }


void sendUnlockCommand() {
  byte hexCommand[] = { 0x46, 0x43, 0x11, 0x01, 0x00, 0x08, 0x4C, 0x49, 0x4D, 0x45, 0x42, 0x49, 0x4B, 0x45, 0xBE, 0x8A };
  Serial.write(hexCommand, sizeof(hexCommand));
}