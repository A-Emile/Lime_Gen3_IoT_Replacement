#include "CRC16.h"
#include "CRC.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define RXD2 16
#define TXD2 17
//GPIO
const int LOCK_PIN = 13;
#define BUZZZER_PIN  14
const int ledPin = 15;

// setting PWM properties for lights
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

uint8_t lightIsOn = 0; // Lightning status

BLEServer *pServer = NULL;
BLECharacteristic *pSpeedCharacteristic;
BLECharacteristic *pRxCharacteristic;

// Status
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
uint8_t isUnlocked = 0;
uint8_t unlockForEver = 0;
float speed = 0;
byte battery = 0x00;

// BLIE
#define SERVICE_UUID "653bb0e0-1d85-46b0-9742-3b408f4cb83f"
#define CHARACTERISTIC_UUID_LOCK "00c1acd4-f35b-4b5f-868d-36e5668d0929"
#define CHARACTERISTIC_UUID_SPEED "2d8b4b10-f938-4a25-a12e-b32d35fbbdb7"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};