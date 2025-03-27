#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>

// --- Configuration ---
// Define pin counts (must match sender)
const int numPots = 6;
const int numSwitches = 6;

// --- Data Structure ---
// Structure to hold sensor data. MUST match the sender's structure.
typedef struct SensorData {
  uint16_t potValues[numPots];
  bool switchStates[numSwitches];
} SensorData;

// Create an instance of the structure to store received data
SensorData receivedData;

// --- Callback Function ---
// Function called when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  // Check if the data length matches the structure size
  if (len == sizeof(receivedData)) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));

    // Print received data to Serial Monitor
    Serial.println("--- Received Data ---");

    // Print Potentiometer Values
    Serial.print("Pots: ");
    for (int i = 0; i < numPots; i++) {
      Serial.print(receivedData.potValues[i]);
      if (i < numPots - 1) Serial.print(", ");
    }
    Serial.println(); // New line after pots

    // Print Switch States
    Serial.print("Switches: ");
    for (int i = 0; i < numSwitches; i++) {
      Serial.print(receivedData.switchStates[i] ? "ON" : "OFF"); // Print ON/OFF for clarity
      // Or just print 1/0: Serial.print(receivedData.switchStates[i]);
      if (i < numSwitches - 1) Serial.print(", ");
    }
    Serial.println(); // New line after switches
    Serial.println("---------------------");

  } else {
    Serial.print("Received data of incorrect size: ");
    Serial.print(len);
    Serial.print(" expected: ");
    Serial.println(sizeof(receivedData));
  }
}

// --- Setup Function ---
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ESP-NOW Receiver");

  // 1. Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.print("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress()); // Print this MAC address!

  // 2. Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW Initialized.");

  // 3. Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receive Callback Registered.");
  Serial.println("Setup Complete. Waiting for data...");
}

// --- Loop Function ---
void loop() {
  // No need to do anything here for this example.
  // All the action happens in the OnDataRecv callback.
  // You could add other tasks here if needed.
  delay(10); // Small delay to prevent watchdog timer issues if loop is truly empty
}