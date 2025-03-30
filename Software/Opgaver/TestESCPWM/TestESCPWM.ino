#include <ESP32Servo.h>

Servo esc;
int escPin = 16;

const int minThrottle = 1100; // Don't go below 1000 µs
const int maxThrottle = 2400; // Your maximum value

void setup() {
  esc.attach(escPin);
  esc.writeMicroseconds(minThrottle); // ESC arm position
  delay(5000); // Allow ESC to arm
}

void loop() {
  // Ramp throttle smoothly up
  for(int pwm = minThrottle; pwm <= maxThrottle; pwm += 10) {
    esc.writeMicroseconds(pwm);
    delay(20);
  }

  delay(2000); // hold max throttle

  // Ramp throttle smoothly down
  for(int pwm = maxThrottle; pwm >= minThrottle; pwm -= 10) {
    esc.writeMicroseconds(pwm);
    delay(20);
  }

  delay(2000); // hold min throttle
}