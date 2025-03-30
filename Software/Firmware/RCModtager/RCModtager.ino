#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>
#include <ESP32Servo.h>

// --- Configuration ---
// Define pin counts (must match sender)
const int numPots = 4;
const int numSwitches = 7;

// --- Data Structure ---
// Structure to hold sensor data. MUST match the sender's structure.
typedef struct SensorData {
  uint16_t potValues[numPots];
  bool switchStates[numSwitches];
} SensorData;

// Create an instance of the structure to store received data
SensorData receivedData;

Servo servo1;  // Servo on GPIO 14
Servo servo2;  // Servo on GPIO 16

void UpdateServo(){
    // Update servo positions based on potValues (assuming 0-1023 input range)
  int servoAngle1 = map(receivedData.potValues[0], 0, 1023, 0, 180);
  int servoAngle2 = map(receivedData.potValues[1], 0, 1023, 0, 180);

  servo1.write(servoAngle1);
  servo2.write(servoAngle2);
}


// --- Callback Function ---
// Function called when data is received
//void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  //const uint8_t *mac_addr = recv_info->src_addr;
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
    // Allow allocation of GPIO pins for ESP32 Servo control
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(50); // standard 50 Hz servo
  servo2.setPeriodHertz(50);

  // Servo initialization

  servo1.attach(14);
  servo2.attach(16);
  // GPIO outputs initialization
  pinMode(15, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);

  // 3. Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receive Callback Registered.");
  Serial.println("Setup Complete. Waiting for data...");
  //wait for 1 sec after first data to set zeroo on servo
}

// --- Loop Function ---
void loop() {
  // No need to do anything here for this example.
  // All the action happens in the OnDataRecv callback.
  // You could add other tasks here if needed.
  delay(10); // Small delay to prevent watchdog timer issues if loop is truly empty
}