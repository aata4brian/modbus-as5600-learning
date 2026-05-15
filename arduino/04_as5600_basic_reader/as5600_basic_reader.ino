#include <Wire.h>

// ======================================
// AS5600 REGISTER
// ======================================
#define AS5600_ADDR    0x36
#define AS5600_ANGLE_H 0x0E

unsigned long lastRead = 0;

// ======================================
// READ AS5600 ANGLE
// ======================================
uint16_t readAS5600Angle() {

  // pilih register angle high
  Wire.beginTransmission(AS5600_ADDR);

  Wire.write(AS5600_ANGLE_H);

  // false = jangan release bus
  Wire.endTransmission(false);

  // request 2 byte
  Wire.requestFrom(AS5600_ADDR, 2);

  if (Wire.available() >= 2) {

    uint8_t highByte = Wire.read();
    uint8_t lowByte  = Wire.read();

    // gabungkan high dan low byte
    uint16_t angle =
      ((uint16_t)highByte << 8)
      | lowByte;

    // masking 12-bit
    angle &= 0x0FFF;

    return angle;
  }

  return 0;
}

void setup() {

  Serial.begin(115200);

  Wire.begin();

  Serial.println(
    "AS5600 BASIC READER"
  );
}

void loop() {

  // baca tiap 100ms
  if (millis() - lastRead >= 100) {

    lastRead = millis();

    uint16_t angle =
      readAS5600Angle();

    // konversi ke degree
    float degree =
      (angle * 360.0)
      / 4096.0;

    Serial.print("ANGLE RAW: ");

    Serial.print(angle);

    Serial.print(" | DEGREE: ");

    Serial.println(degree);
  }
}
