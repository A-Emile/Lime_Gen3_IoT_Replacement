# Lime_Gen3_IoT_Replacement
<b>Note:</b> This project is not endorsed or supported by Lime or any affiliated companies. Only do this on legally obtained scooter that you own! You can often buy them on auctions.

![cover](https://raw.githubusercontent.com/A-Emile/Lime_Gen3_IoT_Replacement/main/iot_original.png)

The goal of this project is to replace the IoT of the Lime Gen 3 with a custom one, so we can controll it with our own app.
If you find out more about the communication, please submit it here.

I am currently working on the PCB and the App. As soon as I have a working prototype, I will published it here.


## Controller Communication
To unlock the controller, the blue wire that is connected to the IoT needs to be supplied with 3.3V, and the hex command `4643110100084C494D4542494B45BE8A` needs to be sent at a baudrate of 9600 every 500ms. Cutting the 3.3V supply and stopping the command transmission will power off the controller after approximately 60 seconds.

The command sent by the controller to the IoT consists of 42 bytes. The 9th byte represents the speed, and the 20th byte represents the battery level. The last two bytes of the command are a CRC-16/XMODEM checksum.

| Byte | Meaning |
|--|--|
| 9 | Speed |
| 20 | Batttery |
| last two bytes | CRC-16/XMODEM checksum |


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


#### Status Bytes:
| Byte | Status |
|--|--|
| 21 | Scan To Ride |
| 22 | Unavailable |
| 23 | Paused |
| 24 | Locked |
| 25 | Done |
| 26 | Charging |
| 31 | Driving |
| 41 | Driving Low Battery |
| 42 | Driving Alert |
| 43 | Driving No Parking |
| 44 | Driving No Riding |
| 45 | Driving Max Speed |
| 51 | Upgrading |
