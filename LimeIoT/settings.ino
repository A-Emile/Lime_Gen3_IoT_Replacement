class SettingsBLECallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    uint8_t data[value.size()];
    memcpy(data, value.c_str(), value.size());

    switch (data[0]) {
      case 0x01:
        // handle max_speed settings
        max_speed = int(data[1]);
        break;
      case 0x02:
        // handle alarm settings
        alarm_delay = int(data[1]) * 10;
        alarm_freq = int(data[2]) * 100;
        alarm_reps = int(data[3]);
        break;
      case 0x03:
        customDisplayStatus = data[1] == 0x00 ? "" : String(data[1], HEX);
        break;
      case 0x4:
        sendDisplayLED(green, off);
        break;
      case 0x46:
        if (data[1] == 0x43) {
          sendControllerCommand(data, value.size());
        }
        break;
    }
    byte settingsByte[] = { max_speed, alarm_delay, alarm_freq, alarm_reps };
    pSettingsCharacteristic->setValue(settingsByte, sizeof(settingsByte));
    pSettingsCharacteristic->notify();
  }
};
