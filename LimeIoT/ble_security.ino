class MySecurity : public BLESecurityCallbacks
{

  uint32_t onPassKeyRequest()
  {
    ESP_LOGI(LOG_TAG, "PassKeyRequest");
    return 123456789;
  }
  void onPassKeyNotify(uint32_t pass_key)
  {
    ESP_LOGI(LOG_TAG, "The passkey Notify number:%d", pass_key);
  }
  bool onConfirmPIN(uint32_t pass_key)
  {
    ESP_LOGI(LOG_TAG, "The passkey YES/NO number:%d", pass_key);
    vTaskDelay(5000);
    return true;
  }
  bool onSecurityRequest()
  {
    ESP_LOGI(LOG_TAG, "SecurityRequest");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
  {
    ESP_LOGI(LOG_TAG, "Starting BLE work!");
    playMP3("/connected.mp3");
    delay(100);
  }
};
