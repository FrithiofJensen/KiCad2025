#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include <HardwareSerial.h>

// --- Configuration ---
// Define the GPIO pins for potentiometers
const int potPins[] = {17, 18, 3, 4}; //Wroom-S2 version
//const int potPins[] = {4, 5, 17, 18, 14, 16}; //Wroom version
const int numPots = sizeof(potPins) / sizeof(potPins[0]);

// Define the GPIO pins for switches
// NOTE: GPIO 35 is INPUT ONLY
const int switchPins[] = {0, 8, 9, 12, 13, 15, 37}; //WROOM-S2 version
//const int switchPins[] = {2, 12, 13, 15, 35, 19}; //WROOM version
const int numSwitches = sizeof(switchPins) / sizeof(switchPins[0]);

// Update frequency (milliseconds)
const unsigned long updateInterval = 100;

// IMPORTANT: Replace with the MAC address of YOUR RECEIVER ESP32
uint8_t receiverMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast placeholder initially
// Example: uint8_t receiverMac[] = {0x3C, 0x71, 0xBF, 0x12, 0x34, 0x56};

// --- Data Structure ---
// Structure to hold sensor data. MUST match the receiver's structure.
typedef struct SensorData {
  uint16_t potValues[numPots];   // Use uint16_t for 12-bit ADC (0-4095)
  bool switchStates[numSwitches]; // true = pressed, false = not pressed
} SensorData;

// Create an instance of the structure
SensorData myData;
HardwareSerial SerialPort(1);  // UART1 instance

// --- ESP-NOW Variables ---
esp_now_peer_info_t peerInfo;
unsigned long lastUpdateTime = 0;

// --- Callback Function ---
// Function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  // Optional: Print confirmation to Sender's Serial Monitor
  // Serial.print("Last Packet Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// --- Setup Function ---
void setup() {
  SerialPort.begin(115200, SERIAL_8N1, 6, 5); // WROOM-S2
  //Serial.begin(115200);
  SerialPort.println("ESP32 ESP-NOW Sender");

  // 1. Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  SerialPort.print("Sender MAC Address: ");
  SerialPort.println(WiFi.macAddress()); // Print MAC address to help configure receiver

  // 2. Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    SerialPort.println("Error initializing ESP-NOW");
    return;
  }
  SerialPort.println("ESP-NOW Initialized.");

  // 3. Register the send callback function
  esp_now_register_send_cb(OnDataSent);

  // 4. Register peer (the receiver)
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0; // Use Wi-Fi channel 0 for ESP-NOW
  peerInfo.encrypt = false; // No encryption for simplicity

  // 5. Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    SerialPort.println("Failed to add peer");
    return;
  }
  SerialPort.println("Peer Added.");

  // 6. Configure GPIOs
  // Potentiometers (Analog Input) - No specific pinMode needed for ADC1 pins usually
  // Switches (Digital Input with Pull-up)
  for (int i = 0; i < numSwitches; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
  }
  pinMode(38, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);

  SerialPort.println("GPIOs Configured.");
  SerialPort.println("Setup Complete. Starting loop...");
}

// --- Loop Function ---
void loop() {
  // Check if it's time to update and send data
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis(); // Reset the timer

    // Read Potentiometers
    for (int i = 0; i < numPots; i++) {
      myData.potValues[i] = analogRead(potPins[i]);
      // Optional: Add smoothing/filtering here if needed
    }

    // Read Switches
    for (int i = 0; i < numSwitches; i++) {
      // digitalRead is LOW when pressed (connected to GND) with INPUT_PULLUP
      myData.switchStates[i] = (digitalRead(switchPins[i]) == LOW);
    }

    SerialPort.println("--- Sent Data ---");
    SerialPort.print("Pots: ");
    for (int i = 0; i < numPots; i++) {
      SerialPort.print(myData.potValues[i]);
      if (i < numPots - 1) SerialPort.print(", ");
    }
    SerialPort.println();
    SerialPort.println("---------------------");

    // Send data via ESP-NOW
    esp_err_t result = esp_now_send(receiverMac, (uint8_t *) &myData, sizeof(myData));

    // Optional: Check send result immediately (also handled by callback)
    // if (result == ESP_OK) {
    //   Serial.println("Sent with success");
    // } else {
    //   Serial.print("Error sending the data: ");
    //   Serial.println(esp_err_to_name(result));
    // }
  }
  // Can add a small delay here if needed, but millis() approach is better
  // delay(1); // Small delay to yield CPU if other tasks are running
}
