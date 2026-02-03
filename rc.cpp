#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// --- Pin Definitions (From your image) ---
const int IN1 = 26; // Left Motor Forward
const int IN2 = 27; // Left Motor Backward
const int IN3 = 14; // Right Motor Forward
const int IN4 = 12; // Right Motor Backward
const int ENA = 25; // Left Motor Speed (PWM)
const int ENB = 33; // Right Motor Speed (PWM)

// --- PWM Properties ---
const int freq = 30000;
const int pwmChannelLeft = 0;
const int pwmChannelRight = 1;
const int resolution = 8; // 8-bit resolution (0-255)

// Default Speed (0-255)
int motorSpeed = 255; 

void setup() {
  Serial.begin(115200);
  
  // Name your Bluetooth device
  SerialBT.begin("Saihaj_RC_Car"); 
  Serial.println("Bluetooth Started! Ready to pair...");

  // Set pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Configure PWM for speed control (ESP32 Specific)
  // Note: If you use ESP32 Core v3.0+, use ledcAttach instead.
  ledcSetup(pwmChannelLeft, freq, resolution);
  ledcSetup(pwmChannelRight, freq, resolution);
  
  // Attach PWM channels to ENA/ENB pins
  ledcAttachPin(ENA, pwmChannelLeft);
  ledcAttachPin(ENB, pwmChannelRight);
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(command);

    // Speed Control Logic (Chars '0' to '9')
    if (command >= '0' && command <= '9') {
      int speedIndex = command - '0';
      if (speedIndex == 0) motorSpeed = 0;
      else motorSpeed = map(speedIndex, 1, 9, 80, 255); // Map 1-9 to PWM range
      Serial.print("Speed set to: ");
      Serial.println(motorSpeed);
    }
    
    // Direction Logic
    switch (command) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopCar(); break;
    }
  }
}

// --- Movement Functions ---

void moveForward() {
  ledcWrite(pwmChannelLeft, motorSpeed);
  ledcWrite(pwmChannelRight, motorSpeed);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  ledcWrite(pwmChannelLeft, motorSpeed);
  ledcWrite(pwmChannelRight, motorSpeed);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  // Spin Left: Left motor back, Right motor forward
  ledcWrite(pwmChannelLeft, motorSpeed);
  ledcWrite(pwmChannelRight, motorSpeed);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  // Spin Right: Left motor forward, Right motor back
  ledcWrite(pwmChannelLeft, motorSpeed);
  ledcWrite(pwmChannelRight, motorSpeed);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(pwmChannelLeft, 0);
  ledcWrite(pwmChannelRight, 0);
}