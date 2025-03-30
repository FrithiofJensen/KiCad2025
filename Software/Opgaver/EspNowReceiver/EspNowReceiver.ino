#include <esp_now.h>
#include <WiFi.h>

// Define the GPIO pin for the LED
// Usually GPIO 2 is the built-in LED on many ESP32 boards
const int ledPin = 2; // Change this if your LED is on a different pin

// Variable to store the current LED state
volatile bool ledState = LOW; // Use volatile as it's changed in an interrupt context (callback)

// Callback function that will be executed when data is received
// Uses the updated signature for recent ESP32 core versions
void OnDataRecv(const esp_now_recv_info_t * info, const uint8_t *incomingData, int len) {
  Serial.print("Bytes received: ");
  Serial.println(len);

  // Optional: Print sender MAC address
  // char macStr[18];
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //          info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  // Serial.print("Received data from: "); Serial.println(macStr);

  // We expect only 1 byte (0 or 1)
  if (len == 1) {
    uint8_t receivedState = incomingData[0];
    Serial.print("Data: ");
    Serial.println(receivedState);

    // Update the LED state based on received data
    if (receivedState == 1) {
      ledState = HIGH;
      Serial.println("Turning LED ON");
    } else if (receivedState == 0) {
      ledState = LOW;
      Serial.println("Turning LED OFF");
    } else {
      Serial.println("Received unknown data.");
    }
    // Control the LED physically
    digitalWrite(ledPin, ledState);

  } else {
     Serial.println("Received data of unexpected length.");
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("ESP-NOW Receiver");

  // Configure the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Optional: Print receiver MAC address for confirmation
  // Serial.print("Receiver MAC Address: ");
  // Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);

   Serial.println("ESP-NOW Initialized. Waiting for data...");
}

void loop() {
  // Nothing needed here for basic receiving
  // Actions happen in the OnDataRecv callback
  delay(10); // Small delay to be nice to the system
}