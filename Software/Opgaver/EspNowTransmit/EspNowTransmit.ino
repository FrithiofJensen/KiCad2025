#include <esp_now.h>
#include <WiFi.h>

// ***************************************************************************
// REPLACE WITH THE ACTUAL MAC ADDRESS OF THE RECEIVER ESP32
// ***************************************************************************
uint8_t receiverMac[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
// Example: If MAC is 3C:71:BF:12:34:56, use:
// uint8_t receiverMac[] = {0x3C, 0x71, 0xBF, 0x12, 0x34, 0x56};
// ***************************************************************************

esp_now_peer_info_t peerInfo;

// Variable to store the state we want the receiver's LED to be in
bool targetLedState = false;

// Callback function executed when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.println("ESP-NOW Sender");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Optional: Print sender MAC address
  // Serial.print("Sender MAC Address: ");
  // Serial.println(WiFi.macAddress());


  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback function
  esp_now_register_send_cb(OnDataSent);

  // Register peer (the receiver)
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0; // Use current Wi-Fi channel
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Peer added");
}

void loop() {
  // Toggle the target state for the receiver's LED
  targetLedState = !targetLedState;

  // Prepare data to send (1 byte: 0 for OFF, 1 for ON)
  uint8_t dataToSend = targetLedState ? 1 : 0;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(receiverMac, &dataToSend, sizeof(dataToSend));

  if (result == ESP_OK) {
    Serial.print("Sent: "); Serial.println(targetLedState ? "ON" : "OFF");
  } else {
    Serial.print("Error sending data: "); Serial.println(result);
  }

  // Wait 500 milliseconds before sending the next command
  // This results in the receiver's LED being ON for 500ms and OFF for 500ms (1 sec total cycle)
  delay(500);
}