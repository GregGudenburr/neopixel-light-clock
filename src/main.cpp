
//board xioa_esp32c3


/**
 * NeoPixel Clock with WiFi and NTP Synchronization
 * 
 * This code creates a clock using a NeoPixel LED strip, synchronizing time via WiFi and NTP.
 * It displays hours, minutes, and seconds using different colored LEDs.
 * 
 * Hardware: ESP32 board, NeoPixel LED strip
 * Libraries: Adafruit_NeoPixel, WiFi, time
 */

#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <time.h>
#include "wifi_credentials.h"
#include <boardinfo.h>

void connectToWiFi();
void pulseBlue();
void setDefaultTime();
void updateClock();
void updateNTP();

// Pin and LED configuration
#define LED_PIN 10
#define NUM_LEDS 60

// Initialize NeoPixel strip
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// WiFi credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Clock face configuration
const int ledOffset = 27; // LED number at 12 o'clock position (start with 0)

// Color definitions for clock hands
const uint32_t hourColor    = strip.Color(128, 50, 35);  
const uint32_t dimHourColor = strip.Color(8, 4, 2);     
const uint32_t minuteColor  = strip.Color(192, 164, 164);
const uint32_t secondColor  = strip.Color(16, 16, 64);   

// NTP server and time zone configuration
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;  // GMT offset in seconds (e.g., -18000 for EST)21600 for texas
const int   daylightOffset_sec = 3600;  // Daylight saving time offset in seconds

// Time tracking variables
unsigned long lastNTPUpdate = 0;
const unsigned long NTP_UPDATE_INTERVAL = 3600000; // 1 hour in milliseconds
struct tm timeinfo;

// WiFi connection timeout variables
unsigned long connectionStartTime;
const unsigned long connectionTimeout = 120000; // 2 minutes in milliseconds

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

   printBoardInfo();

  connectionStartTime = millis();
  connectToWiFi();
  if (WiFi.status() == WL_CONNECTED) {
    updateNTP();
  }
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED) {
    // Check if it's time for an NTP update
    if (millis() - lastNTPUpdate >= NTP_UPDATE_INTERVAL) {
      updateNTP();
    }
    updateClock();
  } else if (millis() - connectionStartTime < connectionTimeout) {
    pulseBlue(); // Show connecting animation
  } else {
    setDefaultTime(); // Show default time if connection fails
    delay(10000);
    ESP.restart(); // Restart the ESP32 to attempt reconnection
  }
  delay(50); // Short delay to control animation speed
}

/**
 * Updates the time from the NTP server
 */
void updateNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("NTP update requested");
  lastNTPUpdate = millis();
}

/**
 * Attempts to connect to WiFi
 */
void connectToWiFi() {
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED && millis() - connectionStartTime < connectionTimeout) {
    pulseBlue();
    delay(50);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" CONNECTED");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  } else {
    Serial.println(" FAILED");
  }
}

/**
 * Displays a pulsing blue lights to indicate connection attempt
 */
void pulseBlue() {
  static int brightness = 0;
  static int direction = 5;
  
  brightness += direction;
  if (brightness <= 0 || brightness >= 255) {
    direction = -direction;
  }
  
  strip.setPixelColor((0  + ledOffset) % 60, strip.Color(0, 0, brightness)); // 12 o'clock LED
  strip.setPixelColor((15 + ledOffset) % 60, strip.Color(0, 0, brightness)); // 3 o'clock LED
  strip.setPixelColor((30 + ledOffset) % 60, strip.Color(0, 0, brightness)); // 6 o'clock LED
  strip.setPixelColor((45 + ledOffset) % 60, strip.Color(0, 0, brightness)); // 9 o'clock LED

  strip.show();
}

/**
 * Sets a default time display (red light at 6 o'clock) when unable to connect
 */
void setDefaultTime() {
  strip.clear();
  strip.setPixelColor((30 + ledOffset) % 60, strip.Color(255, 0, 0)); // Red at 6 o'clock
  strip.show();
}

/**
 * Updates the clock display based on the current time
 */
void updateClock() {
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  int hours = timeinfo.tm_hour % 12;
  int minutes = timeinfo.tm_min;
  int seconds = timeinfo.tm_sec;

  strip.clear();

  // Set LED colors in reverse priority (hour last to ensure visibility)

  // Set hour-adjacent LEDs (to make hour "hand" look wider)
  int hourLED = (hours * 5 + minutes / 12 + ledOffset) % 60;
  strip.setPixelColor((hourLED + 1) % 60, dimHourColor);
  strip.setPixelColor((hourLED + 59) % 60, dimHourColor);
 
  // Set second LED
  int secondLED = (seconds + ledOffset) % 60;
  strip.setPixelColor(secondLED, secondColor);

  // Set minute LED 
  int minuteLED = (minutes + ledOffset) % 60;
  strip.setPixelColor(minuteLED, minuteColor);

  // Set hour LED 
  strip.setPixelColor(hourLED, hourColor);

  strip.show();
}
