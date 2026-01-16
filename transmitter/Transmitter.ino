// Include Libraries
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <MPU6050_light.h>

// Receiver MAC Address
uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x51, 0xC2, 0xBF};

// Struct to send data
typedef struct {
  bool f;
  bool b;
  bool l;
  bool r;
} message;
message data;

// Create an object of the predefined class MPU6050
MPU6050 mpu(Wire);

// Initialize global variables
unsigned long timer = 0;
float x, y;
bool front = false, back = false, left = false, right = false;

// Variables for connection status
bool isConnected = false;
unsigned long lastSentTime = 0;

// Callback function
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if (sendStatus == 0) {
    isConnected = true;
    Serial.println("Delivery Success: Connected to Receiver");
  } else {
    isConnected = false;
    Serial.println("Delivery Fail: Not Connected to Receiver");
  }
}

void setup() {
  Serial.begin(9600);

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error Initializing ESP-NOW");
    return;
  }

  // Register the send callback function
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // MPU6050 setup
  Wire.begin(4, 5); // GPIO 4 (SDA) and GPIO 5 (SCL)

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) {
    delay(100); // Retry if initialization fails
  }

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // Gyro and accelerometer
  Serial.println("Done!\n");
}

void loop() {
  // Get data from MPU6050
  mpu.update();

  if ((millis() - timer) > 10) { // Get data every 10ms
    x = mpu.getAngleX();
    y = mpu.getAngleY();

    // Process data
    back = (x >= 30);
    front = (x <= -30);
    right = (y >= 30);
    left = (y <= -30);

    timer = millis();

    data.f = front;
    data.b = back;
    data.l = left;
    data.r = right;

    // Send message
    esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(data));
    lastSentTime = millis();

    // Print sensor data
    Serial.print("Sending Data: ");
    Serial.print("Front: "); Serial.print(front);
    Serial.print(", Back: "); Serial.print(back);
    Serial.print(", Left: "); Serial.print(left);
    Serial.print(", Right: "); Serial.println(right);
  }

  // Connection status
  if (millis() - lastSentTime > 500) { // Check connection every 500ms
    if (isConnected) {
      Serial.println("Status: Connected to Receiver");
    } else {
      Serial.println("Status: No Connection to Receiver");
    }
  }
}
