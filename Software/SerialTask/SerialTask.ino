#include <WiFi.h>
#include "time.h" // Required for time functions

// --- Wi-Fi Credentials ---
const char* ssid       = "SSID";
const char* password   = "Password";

// --- NTP Settings ---
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov"; // Optional backup server

// --- Set Your Timezone Offset Here ---
const long gmtOffset_sec = 3600;      // Replace with your GMT offset in seconds
const int daylightOffset_sec = 0;  // Replace with your Daylight Saving Time offset in seconds (0 if DST not applicable/active)

// --- Variables ---
struct tm timeinfo; // Structure to hold time data
char timeStringBuff[50]; // Buffer to store formatted time string

// Function prototypes (optional, good practice)
void printLocalTime();
void connectToWiFi();

void setup() {
  Serial.begin(115200);
  Serial.println("\nBooting...");

  connectToWiFi();

  // Configure time provider
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1); // Basic
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2); // With backup server

  Serial.println("Time configured via NTP.");

  // Optional: Wait until time is synchronized for the first time
  Serial.print("Waiting for time sync");
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nTime synchronized!");

  // Print the first time immediately
  printLocalTime();

  // Disconnect WiFi now if you don't need it continuously
  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_OFF);
  // Serial.println("WiFi disconnected.");
}

void loop() {
  printLocalTime();
  delay(1000); // Wait for one second
}

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) { // Timeout after ~10 seconds
        Serial.println("\nFailed to connect to WiFi. Please check credentials or signal.");
        // You might want to restart, or enter a loop, or try other logic here
        while(1) delay(1000);
    }
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


// Function to get and print local time
void printLocalTime() {
  // Get time from ESP32's internal RTC, which is synced by NTP
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    // Could attempt to reconnect WiFi or reconfigure time here if needed
    return;
  }

  // Format time into a string: see https://www.cplusplus.com/reference/ctime/strftime/
  // Example format: "A, B d Y H:M:S" -> "Monday, July 22 2024 15:04:05"
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);

  Serial.println(timeStringBuff);
}