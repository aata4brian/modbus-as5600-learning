#include <Wire.h>
#include <ModbusRtu.h>

// =====================================
// RS485 PIN
// =====================================
#define RS485_EN_PIN 4

// =====================================
// AS5600
// =====================================
#define AS5600_ADDR    0x36
#define AS5600_ANGLE_H 0x0E

#define MODBUS_BAUDRATE 9600

// =====================================
// MODBUS SLAVE ID = 1
// menggunakan hardware UART
// =====================================
Modbus modbusSlave(
  1,
  Serial,
  RS485_EN_PIN
);

// =====================================
// HOLDING REGISTER
// =====================================
// 0 = encoder angle
// 1 = heartbeat
// 2 = AS5600 status
uint16_t holdingRegs[3] = {0, 0, 0};

unsigned long lastEncoderReadTime = 0;
unsigned long lastDebugTime = 0;

// =====================================
// READ AS5600
// =====================================
uint16_t readAS5600(
  bool &isConnected
) {

  Wire.beginTransmission(
    AS5600_ADDR
  );

  Wire.write(
    AS5600_ANGLE_H
  );

  byte error =
    Wire.endTransmission(false);

  if (error != 0) {

    isConnected = false;

    return holdingRegs[0];
  }

  Wire.requestFrom(
    AS5600_ADDR,
    2
  );

  if (Wire.available() >= 2) {

    uint8_t highByte =
      Wire.read();

    uint8_t lowByte =
      Wire.read();

    uint16_t angle =
      ((uint16_t)highByte << 8)
      | lowByte;

    angle &= 0x0FFF;

    isConnected = true;

    return angle;
  }

  isConnected = false;

  return holdingRegs[0];
}

void setup() {

  Wire.begin();

  Wire.setWireTimeout(
    3000,
    true
  );

  // ===================================
  // HARDWARE UART
  // ===================================
  Serial.begin(
    MODBUS_BAUDRATE
  );

  modbusSlave.start();
}

void loop() {

  // ===================================
  // MODBUS PRIORITY
  // ===================================
  modbusSlave.poll(
    holdingRegs,
    3
  );

  // ===================================
  // READ AS5600 EVERY 50 ms
  // ===================================
  if (
    millis()
    - lastEncoderReadTime
    >= 50
  ) {

    lastEncoderReadTime =
      millis();

    bool isConnected = false;

    holdingRegs[0] =
      readAS5600(isConnected);

    holdingRegs[1]++;

    holdingRegs[2] =
      isConnected ? 1 : 0;
  }
}
