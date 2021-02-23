# ESP Compass

Simple compass displayed in web browser.

![Example webpage](/webpage.png)

## Parts

- ESP8266 (e.g. NodeMCU v3)
- compass sensor (e.g. QMC5883L)

## Wiring

![Wiring on breadboard](/fritzing/Sketch_bb.png)

![Wiring schematic](/fritzing/Sketch_schem.png)

## Usage

1. Install serial driver for ESP8266 board.  
   CH340 - <http://www.wch.cn/download/CH341SER_ZIP.html>
2. Install PlatformIO  
   <https://platformio.org/install/ide?install=vscode>
3. Load solution in VSCode
   1. Ensure git submodules are updated
   2. Adjust region settings in `/src/main.h`
   3. Adjust Wi-Fi settings in `/src/wifi.cpp`
   4. `Upload` solution to ESP
   5. `Upload  Filesystem Image` to ESP


Two Wi-Fi operation mode are possible:

1. ESP is Wi-Fi access point (AP). (default)
   - Connect to access point with client.
   - Open Browser <http://192.168.178.1>
2. ESP is Wi-Fi client.
   - Check your router for IP of ESP.

Sensor needs calibration. To calibrate slowly turn sensor while web page is open.
Calibration is automatically stored in device.
To delete calibration use `Earase Flash` in PlatformIO.
An upload will not overwrite the calibration data.