#include <WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop(){
  Serial.println(WiFi.macAddress());
  delay(10000);
}