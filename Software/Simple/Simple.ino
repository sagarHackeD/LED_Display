
#include <ESP8266WiFi.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Font_Data.h"

// Display type for common MAX7219 matrices
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4 // 4 matrices chained

// Pins for ESP8266
#define CS_PIN 15
#define CLK_PIN 14
#define DIN_PIN 13

// WiFi credentials
const char *ssid = "ssid";
const char *password = "password";

// Create Parola instance
MD_Parola matrix =
    MD_Parola(HARDWARE_TYPE, DIN_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,
                     60000); // 19800 = IST offset (GMT +5:30)

bool timeInitialized = false;

void setup() {
  Serial.begin(115200);

  matrix.begin(2); // start Parola need to pass the number of zones otherwise it
                   // wont work
  matrix.displayClear();
  matrix.setInvert(false);
  // matrix.setZoneEffect(0, true, PA_FLIP_LR);
  // matrix.setZoneEffect(0, true, PA_FLIP_UD);

  matrix.setZone(0, 1, 3); 
  matrix.setZone(1, 0, 0);
  matrix.setFont(0, F4x7clean);
  matrix.setFont(1, F3x5std);
  matrix.setIntensity(0, 0); // brightness of matrix
  matrix.setIntensity(1, 0); // brightness of matrix


  // Connect WiFi with bouncing ball animation
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  int connectionAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    drawBouncingBall();
    delay(100);
    connectionAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    matrix.displayClear();

    // Show bouncing ball while syncing NTP
    timeClient.begin();

    int syncAttempts = 0;
    while (syncAttempts < 20) {
      drawBouncingBall();
      timeClient.update();
      delay(200);
      syncAttempts++;
    }
    timeInitialized = true;
    matrix.displayClear();
  } else {
    Serial.println("\nFailed to connect to WiFi");
    matrix.displayText("FAIL", PA_CENTER, 50, 0, PA_PRINT, PA_NO_EFFECT);
  }
}

void loop() {
  if (timeInitialized) {
    timeClient.update();

    int h = timeClient.getHours();
    int m = timeClient.getMinutes();
    int s = timeClient.getSeconds();

    char timeString[6];
    sprintf(timeString, "%02d:%02d", h, m);

    char secString[4];
    sprintf(secString, "%02d", s);

    matrix.displayZoneText(0, timeString, PA_RIGHT, 50, 0, PA_PRINT,PA_SCROLL_UP);
    matrix.displayZoneText(1, secString, PA_CENTER, 50, 0, PA_PRINT,PA_SCROLL_UP);

    if (matrix.displayAnimate()) {
      matrix.displayReset();
    }

    delay(500);
  }
}
