#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include "LOGOXP.h"
#include <time.h>

TFT_eSPI tft = TFT_eSPI();

String ssid = "";
String password = "";

const int logoWidth = 204;
const int logoHeight = 192;
const char* firmwareVersion = "Version 2.0 public mini";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;          // Central European Time +1 hour
const int   daylightOffset_sec = 3600;     // Daylight Saving Time +1 hour

unsigned long lastTimeUpdate = 0;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Initialize NTP time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Synchronizing time with NTP...");

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Enter command:");
  tft.println("W - WiFi configuration");
  tft.println("I - Info");
  tft.println("S - WiFi scan");
  tft.println("R - Reset");
  tft.println("V - Shutdown");
  tft.println("N - Notepad");
  tft.println("E - Erase EEPROM");
  tft.println("DESKTOP - desktop simulation with icons");
}

void loop() {
  // Display current time every second
  if (millis() - lastTimeUpdate > 1000) {
    displayTime();
    lastTimeUpdate = millis();
  }

  if (Serial.available()) {
    String cmdString = Serial.readStringUntil('\n');
    cmdString.trim();

    if (cmdString.equalsIgnoreCase("DESKTOP")) {
      showDesktop();
    } else if (cmdString == "1") {
      Serial.println("WiFi selected");
      configureWiFi();
    } else if (cmdString == "2") {
      Serial.println("Info selected");
      showInfo();
    } else if (cmdString == "3") {
      Serial.println("Scan selected");
      scanWiFi();
    } else if (cmdString == "4") {
      Serial.println("Shutdown selected");
      shutdown();
    } else if (cmdString.equalsIgnoreCase("N")) {
      Serial.println("Notepad");
      notepad();
    } else if (cmdString.equalsIgnoreCase("E")) {
      Serial.println("Erasing EEPROM");
      clearEEPROM();
    } else {
      char cmd = toupper(cmdString.charAt(0));
      switch (cmd) {
        case 'W': configureWiFi(); break;
        case 'I': showInfo(); break;
        case 'S': scanWiFi(); break;
        case 'R': Serial.println("Restarting..."); delay(500); ESP.restart(); break;
        case 'V': Serial.println("Shutting down..."); shutdown(); break;
        default: Serial.println("Unknown command!"); break;
      }
    }
  }
}

void configureWiFi() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Enter SSID:");
  while (!Serial.available()) { delay(10); }
  ssid = Serial.readStringUntil('\n');
  ssid.trim();

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Enter password:");
  while (!Serial.available()) { delay(10); }
  password = Serial.readStringUntil('\n');
  password.trim();

  tft.fillScreen(TFT_BLACK);
  tft.println("Connecting...");
  WiFi.begin(ssid.c_str(), password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  tft.fillScreen(TFT_BLACK);
  if (WiFi.status() == WL_CONNECTED) {
    tft.println("Connected to WiFi!");
    Serial.println("WiFi connected.");
  } else {
    tft.println("Failed to connect.");
    Serial.println("WiFi not connected.");
  }
  delay(2000);
}

void showInfo() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("System info:");
  tft.println(firmwareVersion);
  if (WiFi.status() == WL_CONNECTED) {
    tft.print("IP: ");
    tft.println(WiFi.localIP());
  } else {
    tft.println("Not connected to WiFi.");
  }
  delay(3000);
}

void scanWiFi() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Scanning WiFi networks...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    tft.println("No networks found.");
  } else {
    for (int i = 0; i < n; ++i) {
      tft.print(i + 1);
      tft.print(": ");
      tft.print(WiFi.SSID(i));
      tft.print(" (");
      tft.print(WiFi.RSSI(i));
      tft.println(" dBm)");
      delay(100);
    }
  }
  delay(3000);
}

void shutdown() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Shutting down system...");
  delay(200);
  tft.println(" ");
  ESP.deepSleep(0); // Or another shutdown method depending on hardware
}

void showDesktop() {
  tft.fillScreen(TFT_CYAN);

  // Sun
  tft.fillCircle(tft.width() - 40, 40, 20, TFT_YELLOW);

  // Clouds
  tft.fillCircle(40, 30, 10, TFT_WHITE);
  tft.fillCircle(50, 25, 12, TFT_WHITE);
  tft.fillCircle(60, 30, 10, TFT_WHITE);
  tft.fillCircle(90, 40, 8, TFT_WHITE);
  tft.fillCircle(98, 35, 10, TFT_WHITE);
  tft.fillCircle(108, 40, 8, TFT_WHITE);

  // Bottom bar
  tft.fillRect(0, tft.height() - 40, tft.width(), 40, TFT_GREEN);

  uint16_t frameColor = TFT_WHITE;
  int iconSize = 60;
  int margin = 20;

  // 1. WiFi icon – manually drawn
  int x1 = margin, y1 = margin;
  tft.drawRect(x1, y1, iconSize, iconSize, frameColor);
  int cx = x1 + 30;
  int cy = y1 + 40;
  tft.drawCircle(cx, cy, 2, TFT_BLACK);     // dot
  tft.drawCircle(cx, cy, 10, TFT_BLACK);    // 1st wave
  tft.drawCircle(cx, cy, 16, TFT_BLACK);    // 2nd wave
  tft.drawCircle(cx, cy, 22, TFT_BLACK);    // 3rd wave

  // 2. Info icon
  int x2 = x1 + iconSize + margin;
  tft.drawRect(x2, y1, iconSize, iconSize, frameColor);
  tft.fillCircle(x2 + 30, y1 + 20, 4, TFT_BLACK); // head of "i"
  tft.fillRect(x2 + 28, y1 + 28, 4, 20, TFT_BLACK); // body of "i"

  // 3. Scan – magnifying glass
  int x3 = x2 + iconSize + margin;
  tft.drawRect(x3, y1, iconSize, iconSize, frameColor);
  tft.drawCircle(x3 + 25, y1 + 25, 15, TFT_BLACK); // glass
  tft.drawLine(x3 + 35, y1 + 35, x3 + 50, y1 + 50, TFT_BLACK); // handle

  // 4. Power off – power symbol
  int x4 = margin;
  int y2 = y1 + iconSize + margin;
  tft.drawRect(x4, y2, iconSize, iconSize, frameColor);
  tft.drawCircle(x4 + 30, y2 + 30, 20, TFT_BLACK);  // circle
  tft.drawLine(x4 + 30, y2 + 10, x4 + 30, y2 + 30, TFT_BLACK);  // top line
}

void notepad() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Notepad - type text");
  tft.println("End: ENTER + '.' on new line");

  String note = "";
  while (true) {
    if (Serial.available()) {
      String line = Serial.readStringUntil('\n');
      line.trim();
      if (line == ".") break;

      note += line + "\n";

      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
      tft.println("Notepad - type text");
      tft.println("End: ENTER + '.' on new line");
      tft.println();
      tft.println(note);
    }
    delay(10);
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Text saved:");
  tft.println(note);
  delay(3000);
}

void clearEEPROM() {
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0xFF);  // Clear to 0xFF (typical erased value)
  }
  EEPROM.commit();

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("EEPROM erased!");
  delay(2000);

  Serial.println("EEPROM has been erased.");
}

void displayTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, tft.height() - 16);
  tft.fillRect(0, tft.height() - 16, tft.width(), 16, TFT_BLACK); // clear old time
  char timeStr[16];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  tft.print("Time: ");
  tft.print(timeStr);
}

