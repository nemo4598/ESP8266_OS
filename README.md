# ESP8266 Mini OS v2.0

A lightweight micro-operating system for ESP8266 with TFT display interface and serial command control.  
Includes Wi-Fi management, clock display, notepad, icon-based desktop UI, and more.

![ESP8266 + TFT Display](https://github.com/nemo4598/ESP8266_OS/blob/main/IMG_20250710_135753.jpg)

## 📦 Features

- ⏲️ Real-time clock synced via NTP
- 🌐 Wi-Fi configuration and scanner
- 🖥️ Simulated graphical desktop with icons (drawn manually)
- 📝 Notepad for basic text input via Serial
- ℹ️ Device info display (IP address, firmware version)
- ⚙️ EEPROM reset
- 🛑 Shutdown (deep sleep)

## 🛠️ Hardware Requirements

- ESP8266 board (NodeMCU / Wemos D1 mini / similar)
- TFT Display (compatible with `TFT_eSPI` library, e.g., ILI9341 or ST7735)
- Optional: UART-to-USB adapter for Serial input/output

## 🧰 Libraries Used

- [`ESP8266WiFi`](https://arduino-esp8266.readthedocs.io/)
- [`TFT_eSPI`](https://github.com/Bodmer/TFT_eSPI)
- `EEPROM.h`
- `time.h`

## 🚀 Getting Started

1. **Install the required libraries** in the Arduino IDE.
2. **Configure your `User_Setup.h`** in the `TFT_eSPI` library to match your screen.
3. Flash the code to your ESP8266.
4. Open the **Serial Monitor** at 115200 baud.
5. Use the following commands:

## 💻 Serial Commands

| Command     | Description                         |
|-------------|-------------------------------------|
| `W`         | Wi-Fi configuration                 |
| `I`         | Show system info                    |
| `S`         | Scan Wi-Fi networks                 |
| `R`         | Restart the ESP                     |
| `V`         | Shutdown (deep sleep)               |
| `N`         | Notepad (text input)                |
| `E`         | Erase EEPROM                        |
| `DESKTOP`   | Show graphical desktop interface    |

## 📷 Screenshots

> Add your photos of the TFT display running the project here!

## 📄 License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.

---

## 🤖 Credits

Developed by **Kayen**  
Version: `2.0 public mini`  
Inspired by desktop UIs and CLI-based micro operating systems.


