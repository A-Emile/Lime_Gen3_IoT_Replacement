#include <CRC16.h>
#include <CRC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>
#include "display.h"

#define SCOOTER_NAME "lme-UJEYGJA"
const char *UPDATER_WIFI_PASSWORD = "123456789";
const uint32_t BLE_PASSWORD = 123456789;

// Set pins
#define LED_BUILTIN 2
#define TX0 1  // UART0 - Controller
#define RX0 3
#define RX1 9  // UART1 - SPI Flash (do not use)
#define TX1 10
#define RX2 16 // UART2 - Display
#define TX2 17
#define RX3 22 // UART3 - Debug
#define TX3 23
#define LOCK_PIN 12
#define BUZZER_PIN 25
#define DISPLAY_PIN GPIO_NUM_21
#define SHOCK_PIN GPIO_NUM_14
#define BOOT_PIN GPIO_NUM_33

// Adafruit I2S Amplifier MAX98357A
#define WCLK_PIN 25
#define BCLK_PIN 26
#define DOUT_PIN 27

BLEServer *pServer = NULL;
BLECharacteristic *pMainCharacteristic;
BLECharacteristic *pSettingsCharacteristic;
BLECharacteristic *pDebugCharacteristic;


// Display Status Codes
const String DISPLAY_STATUS_SCAN = "21";
const String DISPLAY_STATUS_ERROR = "22";
const String DISPLAY_STATUS_PAUSED = "23";
const String DISPLAY_STATUS_LOCKED = "24";
const String DISPLAY_STATUS_DONE = "25";
const String DISPLAY_STATUS_CHARGING = "26";
const String DISPLAY_STATUS_DRIVING = "31";
const String DISPLAY_STATUS_DRIVING_LOW_BATTERY = "41";
const String DISPLAY_STATUS_DRIVING_ALERT = "42";
const String DISPLAY_STATUS_DRIVING_NO_PARKING = "43";
const String DISPLAY_STATUS_DRIVING_NO_RIDING = "44";
const String DISPLAY_STATUS_DRIVING_MAX_SPEED = "45";
const String DISPLAY_STATUS_UPGRADING = "51";


// Controller Codes
byte hearthBeatEscByte[16] = { 0x46, 0x43, 0x11, 0x01, 0x00, 0x08, 0x4C, 0x49, 0x4D, 0x45, 0x42, 0x49, 0x4B, 0x45, 0xBE, 0x8A };
byte onEscByte[9] = { 0x46, 0x43, 0x16, 0x61, 0x00, 0x01, 0xF1, 0xF2, 0x8F };
byte offEscByte[9] = { 0x46, 0x43, 0x16, 0x61, 0x00, 0x01, 0xF0, 0xE2, 0xAE };
byte lightOnEscByte[9] = { 0x46, 0x43, 0x16, 0x12, 0x00, 0x01, 0xF1, 0x2B, 0x26 };
byte lightOffEscByte[9] = { 0x46, 0x43, 0x16, 0x12, 0x00, 0x01, 0xF0, 0x3B, 0x07 };
byte lightBlinkEscByte[9] = { 0x46, 0x43, 0x16, 0x13, 0x00, 0x01, 0x06, 0xC2, 0x6A };


// Status
bool deviceConnected = false;
bool oldDeviceConnected = false;
bool isDisconnected = false;
bool commandIsSending = false;
bool isMP3Playing = false;
bool isBooted = false;
bool isIdle = false;
uint8_t isUnlocked = 0;
uint8_t controllerIsOn = 0;
uint8_t lightIsOn = 0;
uint8_t unlockForEver = 0;
float speed = 0;
uint8_t alarmIsOn = 0;
uint8_t throttle = 1;
byte LEDmode = 0x00;
byte battery = 0x00;
byte isCharging = 0x00;
String customDisplayStatus = "";

// Set Settings
int alarm_delay = 200;
int alarm_freq = 3000;
int alarm_reps = 15;
int max_speed = 28;
RTC_DATA_ATTR byte alarm_cnt = 0;
RTC_DATA_ATTR byte lastBattery = 0x00;

//#define BUTTON_PIN_BITMASK ((1ULL << SHOCK_PIN) | (1ULL << BOOT_PIN))
#define BUTTON_PIN_BITMASK (1ULL << BOOT_PIN)

// BLE
#define SERVICE_UUID "653bb0e0-1d85-46b0-9742-3b408f4cb83f"
#define CHARACTERISTIC_UUID_MAIN "00c1acd4-f35b-4b5f-868d-36e5668d0929"
#define CHARACTERISTIC_UUID_SETTINGS "7299b19e-7655-4c98-8cf1-69af4a65e982"
#define CHARACTERISTIC_UUID_DEBUG "83ea7700-6ad7-4918-b1df-61031f95cf62"

// Display Task
TaskHandle_t UARTTask;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

// UARTTaskCode: read controller and send command to display every 300ms
void UARTTaskCode(void *pvParameters) {
  while (true) {
    // reset ESP32 once a week
    if (millis() > 600000000) {
      ESP.restart();
    }
    if (isUnlocked == 1) {
      if (LEDmode != 0x03 && !alarmIsOn) {
        LEDmode = (LEDmode == 0xC3) ? 0x03 : 0xC3;
        sendDisplayLED(green, blink);
      }
      sendDisplayCommand(speed, battery, customDisplayStatus != "" ? customDisplayStatus : DISPLAY_STATUS_DRIVING);
    } else {
      if (isCharging) {
        if (battery < 100) {
          if (LEDmode != 0x0C && !alarmIsOn) {
            LEDmode = (LEDmode == 0xCC) ? 0x0C : 0xCC;
            sendDisplayLED(yellow, blink);
          }
        } else if (LEDmode != 0x03 && !alarmIsOn) {
            LEDmode = (LEDmode == 0xC3) ? 0x03 : 0xC3;
            sendDisplayLED(green, blink);
        }
        sendDisplayCommand(speed, battery, customDisplayStatus != "" ? customDisplayStatus : DISPLAY_STATUS_CHARGING);
      } else if (deviceConnected) {
        if (LEDmode != 0x01 && !alarmIsOn) {
          LEDmode = (LEDmode == 0xC1) ? 0x01 : 0xC1;
          sendDisplayLED(green, on);
        }
        sendDisplayCommand(speed, battery != 0x00 ? battery : lastBattery, customDisplayStatus != "" ? customDisplayStatus : DISPLAY_STATUS_LOCKED);
      } else {
        if (LEDmode != 0x01 && !alarmIsOn) {
          LEDmode = (LEDmode == 0xC1) ? 0x01 : 0xC1;
          sendDisplayLED(green, on);
        }
        sendDisplayCommand(speed, battery != 0x00 ? battery : lastBattery, customDisplayStatus != "" ? customDisplayStatus : DISPLAY_STATUS_SCAN);
      }
    }
    delay(300);
  }
}
