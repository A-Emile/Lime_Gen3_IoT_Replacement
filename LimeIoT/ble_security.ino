class MySecurity : public NimBLEServerCallbacks
{

  uint32_t onPassKeyRequest()
  {
    return BLE_PASSWORD;
  }
  void onPassKeyNotify(uint32_t pass_key)
  {
    
  }
  bool onConfirmPIN(uint32_t pass_key)
  {
    
    vTaskDelay(5000);
    return true;
  }
  bool onSecurityRequest()
  {
    
    return true;
  }

  void onAuthenticationComplete()
  {
    
    connectedBeeb();
  }
};
