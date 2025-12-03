#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// Display type for common MAX7219 matrices
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4  // 4 matrices chained

// Pins for ESP8266
#define CS_PIN 15
#define CLK_PIN 14
#define DIN_PIN 13

// WiFi credentials
const char* ssid = "ssid";
const char* password = "password";

// Create Parola instance
MD_Parola matrix = MD_Parola(HARDWARE_TYPE, DIN_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // 19800 = IST offset (GMT +5:30)

void setup() {
  Serial.begin(115200);

  matrix.begin();
  matrix.setIntensity(1);
  matrix.displayClear();

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected!");

  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  int h = timeClient.getHours();
  int m = timeClient.getMinutes();

  char timeString[6];
  sprintf(timeString, "%02d:%02d", h, m);

  matrix.displayText(timeString, PA_CENTER, 50, 0, PA_PRINT, PA_NO_EFFECT);

  if (matrix.displayAnimate()) {
    matrix.displayReset();
  }

  delay(500);
}
