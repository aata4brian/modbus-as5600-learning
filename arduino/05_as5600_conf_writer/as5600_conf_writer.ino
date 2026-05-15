#include <Wire.h>

// ======================================
// AS5600
// ======================================
#define AS5600_ADDR 0x36

// CONF REGISTER
#define CONF_HIGH_REG 0x07
#define CONF_LOW_REG  0x08

// ======================================
// WRITE CONF
// ======================================
void writeCONF(
  uint8_t highByte,
  uint8_t lowByte
) {

  Wire.beginTransmission(
    AS5600_ADDR
  );

  // mulai dari CONF HIGH
  Wire.write(CONF_HIGH_REG);

  // tulis high byte
  Wire.write(highByte);

  // tulis low byte
  Wire.write(lowByte);

  byte error =
    Wire.endTransmission();

  if (error == 0) {

    Serial.println(
      "CONF WRITE SUCCESS"
    );
  }
  else {

    Serial.print(
      "CONF WRITE ERROR: "
    );

    Serial.println(error);
  }
}

// ======================================
// READ CONF
// ======================================
void readCONF() {

  Wire.beginTransmission(
    AS5600_ADDR
  );

  Wire.write(CONF_HIGH_REG);

  Wire.endTransmission(false);

  Wire.requestFrom(
    AS5600_ADDR,
    2
  );

  if (Wire.available() >= 2) {

    uint8_t highByte =
      Wire.read();

    uint8_t lowByte =
      Wire.read();

    Serial.print("CONF HIGH: 0x");

    Serial.println(
      highByte,
      HEX
    );

    Serial.print("CONF LOW : 0x");

    Serial.println(
      lowByte,
      HEX
    );
  }
}

void setup() {

  Serial.begin(115200);

  Wire.begin();

  delay(1000);

  Serial.println(
    "AS5600 CONF WRITER"
  );

  // ====================================
  // CONTOH CONFIG
  // ====================================
  // tinggal ganti hex ini
  // ====================================
  writeCONF(
    0x00,
    0x00
  );

  delay(500);

  readCONF();
}

void loop() {

}
