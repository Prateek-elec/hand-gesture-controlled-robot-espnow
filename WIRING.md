# Wiring Notes — Hand Gesture Controlled Bot (ESP8266 + ESP-NOW + MPU6050)

This project has **two different circuits**:

✅ **TRANSMITTER (Hand Unit)** → ESP8266 + MPU6050  
✅ **RECEIVER (Robot Unit)** → ESP8266 + Motor Driver + Motors + Battery  

---

# ✅ A) TRANSMITTER (HAND UNIT) WIRING ✅

## 1) Components Used
- ESP8266 (NodeMCU / Wemos D1 mini)
- MPU6050 (GY-521 Module)
- Battery / power source (Power bank / Li-ion + regulator)
- Switch (optional)

---

## 2) Power Connections (Transmitter)
| Connection | From | To |
|----------|------|----|
| MPU6050 VCC | ESP8266 3.3V | MPU6050 VCC |
| MPU6050 GND | ESP8266 GND | MPU6050 GND |

⚠️ Important Notes:
- MPU6050 should ideally run at **3.3V** with ESP8266.
- Do NOT power MPU6050 from motor battery lines.
- Keep wiring short to reduce noise.

---

## 3) I2C Connections (MPU6050 ↔ ESP8266)
| MPU6050 Pin | ESP8266 GPIO | NodeMCU Pin |
|------------|-------------|------------|
| SDA        | GPIO4       | D2 |
| SCL        | GPIO5       | D1 |

✅ Code matches:
```cpp
Wire.begin(4, 5); // SDA = GPIO4(D2), SCL = GPIO5(D1)
