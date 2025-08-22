# IoT Temperature & Humidity Monitor (ESP8266 + DHT22 + OLED)

This project is a compact IoT system designed to monitor temperature and humidity using a **DHT22 sensor**, transmit the data via an **ESP8266 (ESP-01)** Wi-Fi module, and display readings on both **Adafruit IO** and a local **OLED screen**.

The hardware is built on a **custom-designed PCB**, and the full design process, including schematics, manufacturing files, and design choices, is documented in the final project report.

> **View the full report**: [`IoT Final Report.pdf`](IoT%20Final%20Report.pdf)  
> Note: OLED display functionality was **added after** the report was completed and is **not included** in the PDF.
> Gerber (CAM) files for PCB manufacturing are included as a ZIP archive in this repository.
---

##  Features

- Sends real-time sensor data to **Adafruit IO**
-  Displays temperature and humidity on an **OLED**
-  Updates every 5 seconds
-  Written in Arduino (SoftwareSerial, DHT, U8x8lib)

---

## Files in This Repo

| File                     | Description                                                   |
|--------------------------|---------------------------------------------------------------|
| `tempOLED.ino`           | Final working Arduino code including OLED support             |
| `IoT Final Report.pdf`   | Report with PCB design, schematics, BOM, Adafruit dashboard   |
| `AlOud_Blue.zip`         | Gerber (CAM) files for PCB manufacturing                      |

---

## Quick Setup

1. Connect DHT22 to the designated pin 
2. Connect ESP-01 via SoftwareSerial 
3. Connect OLED via I2C
4. Upload `tempOLED.ino` to the Arduino-compatible board
5. Modify Wi-Fi credentials and Adafruit IO keys in the code

---

##  Future Improvements

- Add battery support
- Add SD card for local logging
- Push alerts when readings exceed thresholds

---

© 2025 Omar AlOud  
University of Delaware – CPEG/ELEG 298
