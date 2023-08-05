void setup() {

  // create a task that will be executed along the loop() function, with priority 2 and executed on core 0
  xTaskCreatePinnedToCore(
    UARTTaskCode,  // Task function.
    "DisplayTask", // name of task.
    4096,          // Stack size of task
    NULL,          // parameter of the task
    2,             // priority of the task
    &UARTTask,     // Task handle to keep track of created task
    0);            // pin task to core 0

  // ESP32 onboard LED
  pinMode(LED_BUILTIN,OUTPUT);

  // Controller
  pinMode(LOCK_PIN, OUTPUT);
  digitalWrite(LOCK_PIN, LOW);
  controllerIsOn = 0;

  // Display LOW = on, HIGH = off
  pinMode(DISPLAY_PIN, OUTPUT);
  digitalWrite(DISPLAY_PIN, LOW);
//  gpio_hold_en(DISPLAY_PIN);
/*
  //Setup sleep wakeup on Touch Pad 3 ( GPIO15 )
  touchSleepWakeUpEnable(T3,40);
  //Setup sleep wakeup to shock sensor
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_14,1);
*/
  // wake on shock sensor
  pinMode(SHOCK_PIN, INPUT_PULLDOWN);
//  rtc_gpio_deinit(SHOCK_PIN);
//  rtc_gpio_pulldown_en(SHOCK_PIN);

  // wake on charger
  pinMode(BOOT_PIN, INPUT);

  // SHOCK_PIN | BOOT_PIN
//  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  /* SPI flash file system
  |--------------|-------|---------------|--|--|--|--|--|
  ^              ^       ^               ^     ^
  Sketch    OTA update   File system   EEPROM  WiFi config (SDK) */
  LittleFS.begin();

  // do not print any debug messages on controller to reduce noise
  Serial.begin(115200, SERIAL_8N1, RX3, TX3);  // swapped -> UART3
  Serial1.begin(9600, SERIAL_8N1, RX0, TX0);   // swapped -> UART0
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2);
  Serial.println("Starting BLE work!");

  BLEDevice::init(SCOOTER_NAME);
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  /*
   * Required in authentication process to provide displaying and/or input passkey or yes/no butttons confirmation
   */
  BLEDevice::setSecurityCallbacks(new MySecurity());
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pMainCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_MAIN,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pMainCharacteristic->setCallbacks(new MainBLECallback());

  pMainCharacteristic->addDescriptor(new BLE2902());

   pDebugCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_MAIN,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pDebugCharacteristic->addDescriptor(new BLE2902());

  pSettingsCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_SETTINGS,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pSettingsCharacteristic->setCallbacks(new SettingsBLECallback());

  pSettingsCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;  //bonding with peer device after authentication
  esp_ble_io_cap_t iocap = ESP_IO_CAP_OUT;                     //set the IO capability to No output No input
  uint8_t key_size = 16;                                       //the key size should be 7~16 bytes
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  //set static passkey
  uint32_t passkey = 123456;
  uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
  uint8_t oob_support = ESP_BLE_OOB_DISABLE;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
  //    esp_ble_gap_set_security_param(ESP_BLE_SM_OOB_SUPPORT, &oob_support, sizeof(uint8_t));
  /* If your BLE device act as a Slave, the init_key means you hope which types of key of the master should distribut to you,
    and the response key means which key you can distribut to the Master;
    If your BLE device act as a master, the response key means you hope which types of key of the slave should distribut to you,
    and the init key means which key you can distribut to the slave. */
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
  Serial.println("Ready!");

  // Play ready sound
  delay(2500);
  tone(BUZZER_PIN, 300, 100);
  delay(100);
  tone(BUZZER_PIN, 400, 100);
  delay(100);
  tone(BUZZER_PIN, 500, 100);
  delay(100);
  noTone(BUZZER_PIN);
/*
  beep(300, 100);
  beep(400, 100);
  beep(500, 100);
*/
  // disable AudioLogger
  Print* audioLogger = &silencedLogger;

  turnOnController();
}
