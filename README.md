# Lime_Gen3_IoT_Replacement
<b>Note:</b> This project is not endorsed or supported by Lime or any affiliated companies. Only do this on legally obtained scooter that you own! You can often buy them on auctions.<br><br>
**The Arduino code has been writting with zero knowledge about how to write proper code and may not work correctly. We are working on a new firmware and pcb wich will fix all the bugs and makes everything easier in the [V2 branch](https://github.com/A-Emile/Lime_Gen3_IoT_Replacement/tree/v2).**

![Screenshot (140)](https://github.com/user-attachments/assets/ad886ee4-42da-4212-b8cb-925b170bc87c)

The goal of this project is to replace the IoT of the Lime Gen 3 with a custom one, so we can control it with our own app.

## How it works
The IoT module gets replaced by an ESP32 microcontroller wich enables us to control the scooter with our app. The app communicates with the ESP32 using BLE. The ESP32 replaces the function of the original IoT while also providing real-time feedback on speed, battery level, and other information.

## Installation
Install the ESP32 add-on for Arduino IDE if you doesnt have already. [Here is a tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

Install the [crc](https://github.com/RobTillaart/CRC) library by robtillaart from the library manager.

Flash the controller with [unlocked firmware](https://cloud.scooterhacking.org/release/lime_dropbox/).

Flash the arduino code from [LimeIoT](https://github.com/A-Emile/Lime_Gen3_IoT_Replacement/tree/main/LimeIoT) folder to the esp32.


<b>Note:</b> The controller only gives you battery voltage (~36V). So you have to use a buck converter to bring it down to 5v for the display and esp32.

Connect the wires:

![LimeIoTConnector](https://user-images.githubusercontent.com/76005215/227743332-2c972cca-d37c-4bcd-b67e-097f84796bc5.jpg)

| Connector | ESP32 |
| -------- | ------- |
| Controller lock  | GPIO 12   |
| Controller RX    | GPIO 1    |
| Controller TX    | GPIO 3    |
| Controller 36v   | Buck converter -> 5V |
| Controller Gnd   | Gnd       |
| Display 5v       | 5V        |
| Display Gnd      | Gnd       |
| Display TX       | GPIO  16  |
| Display RX       | GPIO  17  |

If you want, you can connect the speaker to `GPIO 13`.

## Usage
You can download the app here: [App.apk](https://github.com/A-Emile/Lime_Gen3_IoT_Replacement/raw/main/App.apk)

The default bluetooth password is `123456789`. You can change it in the [ble_security.ino](https://github.com/A-Emile/Lime_Gen3_IoT_Replacement/blob/main/LimeIoT/ble_security.ino) file.

Currently only compatible on android and is only looking good with Material You compatible phones.

If you dont want/can use the app, you can just download a bluetooth terminal app like nRF Connect ([Play Store](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&amp;hl=de&amp;gl=US) | [App Store](https://apps.apple.com/de/app/nrf-connect-for-mobile/id1054362403)) and send the commands yourself:

| Commands | Action |
| -------- | ------- |
| `unlock`  | Unlocks the scooter |
| `lock`    | Turns off the controller |
| `unlockforever`    | Keeps the scooter unlocked when disconnecting |
| `alarm`   | let the scooter beeb (if speaker is connected ) |

## Controller Communication
To unlock the controller, supply 3.3V to the blue wire connected to the IoT and send the command `464316610001F1F28F` to power it on. Once powered on, send the heartbeat `4643110100084C494D4542494B45BE8A` every 500ms. To power off the controller, cut the 3.3V supply and send the command `464316610001F0E2AE`. The baudrate for all commands is `9600`.

The command sent by the controller to the IoT consists of 42 bytes. The 9th byte represents the speed, and the 20th byte represents the battery level. The last two bytes of the command are a CRC-16/XMODEM checksum.

| Byte number | Meaning |
|--|--|
| 9 | Speed |
| 20 | Batttery |
| last two bytes | CRC-16/XMODEM checksum |

#### Example: `46 58 0C FF 00 22 11 00 00 40 00 00 41 3F 60 42 00 FF 44 64 52 00 61 F1 80 00 00 72 01 5C 01 59 82 00 00 00 00 E0 00 00 0A B3`


## Display Communication
- Baudrate: `115200`
- Checksum: `width=8  poly=0x31  init=0x0a  refin=true  refout=true  xorout=0x00  check=0xc1  residue=0x00`

The following table shows the known meaning of the bytes in the commands send to the display:
| Byte | Meaning |
|--|--|
| 12 | Status (see below) |
| 14 | Batttery |
| 16-17 | Speed |
| last byte | checksum |


#### Status Byte Values:
| Byte | Status |
|--|--|
| 0x21 | Scan To Ride |
| 0x22 | Unavailable |
| 0x23 | Paused |
| 0x24 | Locked |
| 0x25 | Done |
| 0x26 | Charging |
| 0x31 | Driving |
| 0x41 | Driving Low Battery |
| 0x42 | Driving Alert |
| 0x43 | Driving No Parking |
| 0x44 | Driving No Riding |
| 0x45 | Driving Max Speed |
| 0x51 | Upgrading |

#### Example: `4C 42 44 43 50 01 10 11 00 09 01 31 01 1E 02 00 CD 01 00 9A`

You can turn off the red LED with the following command: `4C 42 44 43 50 01 10 1B 00 08 03 00 00 00 03 00 00 00`
