# Lime Gen 3 Personal IoT V2 (In active development. Not finished yet)
If you dont want to wait for V2, switch to the [main branche](https://github.com/A-Emile/Lime_Gen3_IoT_Replacement/tree/main)
### !!! This project is not endorsed or supported by Lime or any affiliated companies. Only do this on legally obtained scooters you own. !!!

![Screenshot (137)](https://github.com/user-attachments/assets/22a7ad7d-2851-4104-bd65-90a0eeb4ff31)

The goal of this project is to replace the IoT of the Lime Gen 3 with one that works offline, can be configured via BLE and supports various unlock methods.

## How it works
The IoT module gets replaced with the new IoT wich handles ESC unlocking, configuration and real time infos. Users then can control the scooter with a mobile app. The App is used for configuration like setting speed modes, max power, display ui, LEDs etc.. The user also gets the option to lock, unlock, switch speedmode using a remote wich removes the need to use the phone for everyday use.
The replacement IoT consists of the following crucial parts:
- **DC-DC stepdown converter** for converting the battery voltage to 5v and 3.3v.
- **ESP32 C3** wich handles all the logic.
- **433Mhz Receiver (optional)** for receiving rf commands from the remote.
- **BMA400 Accerelometer (optional)** for detecting illegal movement.
- **MAX98357A DAC (optional)** for playing feedback sounds on the speaker.

## Motivation
The 3th Generation of Lime scooters slowly gets replaced by their 4th Generation. This leads to many dumped gen 3 scooters. They get often selled on local auction. Since its not easely possible to use the scooters with their default hardware, we have created this project, to give them a new life.

## Usage
Follow [this document (coming soon)]() to build the iot or buy a finished plug an play version [here (coming soon)]().<br></br>
Open original IoT, remove it and plug in the new one.<br></br>
You can download the app here: [App.apk](https://github.com/A-Emile/Lime_Gen3_IoT_Replacement/raw/main/App.apk)

The default bluetooth password is `123456789`. You can change it later in the app settings.

## Mobile App
Coming soon!
