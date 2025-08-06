/***************************************************************************
  Modified to send DHT22 temp and humidity data to Adafruit IO using
  an ESP8266 WiFi module and display the readings on an OLED.

  Original authors: Theo Fleck and Rick Martin, Uiversity of Delaware
  Modified: May 2025 by Omar AlOud
 ***************************************************************************/

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "DHT.h"
#include <U8x8lib.h>

#define RESET_PIN 4
#define UART_TX_PIN 3
#define UART_RX_PIN 2
#define DHTPIN 6
#define DHTTYPE DHT22

// OLED (Grove, I2C)
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial espSerial(UART_RX_PIN, UART_TX_PIN);

// Adafruit IO Credentials
String IO_USERNAME = "YOUR_ADAFRUIT_IO_USERNAME";
String IO_KEY = "YOUR_ADAFRUIT_IO_KEY";
String WIFI_SSID = "your_wifi_ssid";
String WIFI_PASS = "your_wifi_password";

// Internal vars
String response;
String subresp;
long int time, updatetime;

void setup() {
  Serial.begin(115200);
  espSerial.begin(9600);

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  delay(1000);
  digitalWrite(RESET_PIN, HIGH);

  dht.begin();

  // Initialize OLED
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clear();
  u8x8.drawString(0, 0, "CPEG/ELEG Demo");
  u8x8.drawString(0, 1, "Setting up WiFi...");
  delay(5000); // show for 5 sec
  u8x8.clear(); // clear OLED after boot message

  time = millis();
  while ((time + 2000) > millis()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
      Serial.print(c);
    }
  }

  espSerial.flush();
  Serial.println("\n\nStarting Setup...");
  Serial.println("Connecting to Adafruit IO");

  response = espSend("get_macaddr", 2000, true);
  response = espSend("get_version", 2000, true);
  response = espSend("wifi_ssid=" + WIFI_SSID, 2000, true);
  response = espSend("wifi_pass=" + WIFI_PASS, 2000, true);
  response = espSend("io_user=" + IO_USERNAME, 2000, true);
  response = espSend("io_key=" + IO_KEY, 2000, false);
  response = espSend("setup_io", 30000, true);

  if (response.indexOf("connected") < 0) {
    Serial.println("Adafruit IO Connection Failed");
    u8x8.drawString(0, 0, "WiFi Failed");
    while (1);
  }

  response = espSend("setup_pubfeed=1,CPEG298Temp", 2000, false);
  response = espSend("setup_pubfeed=2,CPEG298Humidity", 2000, false);

  Serial.println("------ Setup Complete ----------");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    u8x8.clear();
    u8x8.drawString(0, 0, "Sensor Error");
    delay(5000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°C, Hum: ");
  Serial.print(hum);
  Serial.println("%");

  // Display on OLED
  u8x8.clear();
  u8x8.setCursor(0, 0);
  u8x8.print("WiFi: Connected");

  u8x8.setCursor(0, 1);
  u8x8.print("Temp: ");
  u8x8.print(temp, 1);
  u8x8.print(" C");

  u8x8.setCursor(0, 2);
  u8x8.print("Hum: ");
  u8x8.print(hum, 1);
  u8x8.print(" %");

  // Send to Adafruit IO
  response = espSend("send_data=1," + String(temp), 2000, false);
  response = espSend("send_data=2," + String(hum), 2000, false);

  delay(5000);
}

String espSend(String command, const int timeout, boolean debug) {
  response = "";
  char c;
  espSerial.println(command);
  long int time = millis();

  while ((time + timeout) > millis()) {
    while (espSerial.available()) {
      c = espSerial.read();
      response += c;
    }
    if (c == '\n') break;
  }

  response.trim();
  if (debug) {
    Serial.println("Resp: " + response);
  }

  return response;
}
