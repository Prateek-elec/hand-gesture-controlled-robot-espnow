// Include Libraries
#include <espnow.h>
#include <ESP8266WiFi.h>

// Define Motor Control Pins for MDD10A
#define PWM_A 5  // GPIO5 (D1) - PWM signal for Motor A
#define DIR_A 4  // GPIO4 (D2) - Direction control for Motor A
#define PWM_B 12 // GPIO12 (D6) - PWM signal for Motor B
#define DIR_B 14 // GPIO14 (D5) - Direction control for Motor B

// Initialize global variables
bool front = false;
bool back = false;
bool left = false;
bool right = false;

// Struct to receive data (must match the transmitter's struct)
typedef struct {
  bool f;
  bool b;
  bool l;
  bool r;
} message;

message data;

// Callback function to handle incoming data
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  front = data.f;
  back = data.b;
  left = data.l;
  right = data.r;
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize motor control pins
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  pinMode(DIR_B, OUTPUT);

  // Ensure motors are stopped at startup
  carStop();

  // Set Wi-Fi mode to Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW Initialized");
}

void loop() {
  // Handle car movement based on received data
  if (front) {
    carForward();
  } else if (back) {
    carBackward();
  } else if (left) {
    carTurnLeft();
  } else if (right) {
    carTurnRight();
  } else {
    carStop();
  }
}

// Functions for specific movements
void carForward() {
  analogWrite(PWM_A, 64); // 1/4th speed for Motor A
  digitalWrite(DIR_A, HIGH); // Forward direction for Motor A
  analogWrite(PWM_B, 64); // 1/4th speed for Motor B
  digitalWrite(DIR_B, HIGH); // Forward direction for Motor B
}

void carBackward() {
  analogWrite(PWM_A, 64); // 1/4th speed for Motor A
  digitalWrite(DIR_A, LOW); // Reverse direction for Motor A
  analogWrite(PWM_B, 64); // 1/4th speed for Motor B
  digitalWrite(DIR_B, LOW); // Reverse direction for Motor B
}

void carTurnLeft() {
  analogWrite(PWM_A, 64); // Reduced speed for Motor A
  digitalWrite(DIR_A, LOW); // Reverse direction for Motor A
  analogWrite(PWM_B, 64); // Reduced speed for Motor B
  digitalWrite(DIR_B, HIGH); // Forward direction for Motor B
}

void carTurnRight() {
  analogWrite(PWM_A, 64); // Reduced speed for Motor A
  digitalWrite(DIR_A, HIGH); // Forward direction for Motor A
  analogWrite(PWM_B, 64); // Reduced speed for Motor B
  digitalWrite(DIR_B, LOW); // Reverse direction for Motor B
}

void carStop() {
  analogWrite(PWM_A, 0); // Stop Motor A
  analogWrite(PWM_B, 0); // Stop Motor B
}

