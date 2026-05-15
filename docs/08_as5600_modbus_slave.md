# AS5600 + Modbus Slave

---

# Overview

Pada tahap ini:

```text
AS5600 → Arduino Slave → Modbus RTU → Arduino Master
````

Slave:

* membaca encoder AS5600
* menyimpan data ke holding register
* mengirim data ke master

---

# Tujuan Pembelajaran

Mempelajari:

* integrasi AS5600 dengan Modbus
* I2C + Modbus dalam satu loop
* timing issue
* non-blocking system
* SoftwareSerial limitation
* polling architecture

---

# Arsitektur Sistem

```text
AS5600
   ↓ I2C
Arduino Slave
   ↓ RS485
Arduino Master
```

---

# Hardware

* Arduino UNO / Nano
* MAX485
* AS5600
* Breadboard
* Jumper wires

---

# Wiring AS5600

| AS5600 | Arduino   |
| ------ | --------- |
| SDA    | A4        |
| SCL    | A5        |
| VCC    | 3.3V / 5V |
| GND    | GND       |

---

# Wiring MAX485

| Arduino | MAX485  |
| ------- | ------- |
| pin 11  | DI      |
| pin 10  | RO      |
| pin 4   | RE + DE |
| 5V      | VCC     |
| GND     | GND     |

---

# Wiring RS485

| Slave MAX485 | Master MAX485 |
| ------------ | ------------- |
| A            | A             |
| B            | B             |
| GND          | GND           |

---

# Register Map

| Register | Fungsi        |
| -------- | ------------- |
| regs[0]  | angle AS5600  |
| regs[1]  | heartbeat     |
| regs[2]  | status sensor |

---

# Kenapa Menggunakan Holding Register?

Karena Modbus RTU menggunakan:

```text
holding register
```

untuk:

* menyimpan data
* dibaca master
* komunikasi antar node

---

# Program Slave

```cpp
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ModbusRtu.h>

#define MODBUS_RX 10
#define MODBUS_TX 11
#define RS485_EN  4

#define AS5600_ADDR    0x36
#define AS5600_ANGLE_H 0x0E

SoftwareSerial modbusPort(MODBUS_RX, MODBUS_TX);
Modbus slave(1, modbusPort, RS485_EN);

// regs[0] = angle AS5600
// regs[1] = heartbeat
// regs[2] = status AS5600
uint16_t regs[3] = {0, 0, 0};

unsigned long lastAS5600Read = 0;
unsigned long lastDebug = 0;

uint16_t readAS5600Angle(bool &ok) {

  Wire.beginTransmission(AS5600_ADDR);

  Wire.write(AS5600_ANGLE_H);

  byte error =
    Wire.endTransmission(false);

  if (error != 0) {

    ok = false;

    return regs[0];
  }

  Wire.requestFrom(AS5600_ADDR, 2);

  if (Wire.available() >= 2) {

    uint8_t highByte = Wire.read();
    uint8_t lowByte  = Wire.read();

    uint16_t angle =
      ((uint16_t)highByte << 8)
      | lowByte;

    angle &= 0x0FFF;

    ok = true;

    return angle;
  }

  ok = false;

  return regs[0];
}

void setup() {

  Serial.begin(115200);

  Wire.begin();

  Wire.setWireTimeout(3000, true);

  modbusPort.begin(9600);

  modbusPort.listen();

  slave.start();

  Serial.println(
    "SLAVE AS5600 + MODBUS READY"
  );
}

void loop() {

  // =====================================
  // PRIORITAS MODBUS
  // =====================================
  slave.poll(regs, 3);

  // =====================================
  // BACA AS5600 TIAP 50 ms
  // =====================================
  if (
    millis() - lastAS5600Read
    >= 50
  ) {

    lastAS5600Read =
      millis();

    bool ok = false;

    regs[0] =
      readAS5600Angle(ok);

    regs[1]++;

    regs[2] =
      ok ? 1 : 0;
  }

  // =====================================
  // DEBUG SERIAL
  // =====================================
  if (
    millis() - lastDebug
    >= 500
  ) {

    lastDebug =
      millis();

    Serial.print("ANGLE: ");

    Serial.print(regs[0]);

    Serial.print(" | HB: ");

    Serial.print(regs[1]);

    Serial.print(" | AS5600: ");

    Serial.println(
      regs[2]
      ? "OK"
      : "ERR"
    );
  }
}
```

---

# Penjelasan Program

---

# AS5600 Dibaca Lewat I2C

```cpp
Wire.beginTransmission()
Wire.requestFrom()
```

Digunakan untuk:

```text
membaca register angle
```

dari AS5600.

---

# High Byte dan Low Byte

AS5600 mengirim:

* high byte
* low byte

yang digabung menjadi:

```cpp
uint16_t angle
```

---

# Masking 12-bit

```cpp
angle &= 0x0FFF;
```

Karena AS5600 hanya memakai:

```text
12-bit
```

---

# Status Sensor

```cpp
regs[2] = ok ? 1 : 0;
```

Jika pembacaan berhasil:

```text
1 = OK
```

Jika gagal:

```text
0 = ERROR
```

---

# Heartbeat

```cpp
regs[1]++;
```

Digunakan untuk:

* memastikan slave hidup
* memastikan komunikasi berjalan

---

# Masalah Besar yang Terjadi

Saat AS5600 dipasang:

```text
Modbus fail
Serial Monitor freeze
timeout
```

padahal:

* wiring benar
* sensor benar
* Modbus benar

---

# Penyebab Utama

Bukan:

* RS485 rusak
* AS5600 rusak
* MAX485 rusak

melainkan:

```text
timing problem
```

---

# Kenapa Bisa Trouble?

Karena program awal:

```text
membaca AS5600 terus-menerus
```

---

# Akibatnya

CPU sibuk:

* I2C
* Wire.requestFrom()
* Wire.endTransmission()

dan:

```cpp
slave.poll()
```

jarang dipanggil.

---

# Kenapa slave.poll() Penting?

Karena:

```text
slave.poll()
```

adalah:

* penerima request Modbus
* parser packet
* handler Modbus
* response sender

---

# Jika poll() Telat

Master melihat:

```text
TIMEOUT
NO RESPONSE
MODBUS FAIL
```

---

# Masalah Semakin Parah Karena

Modbus menggunakan:

```text
SoftwareSerial
```

---

# SoftwareSerial

SoftwareSerial menggunakan:

```text
bit-banging
```

Artinya:

```text
CPU sendiri membaca timing serial
```

---

# Jadi Saat CPU Sibuk I2C

SoftwareSerial:

```text
kehilangan timing bit
```

---

# Akibatnya

* frame corrupt
* timeout
* response gagal
* Modbus tidak stabil

---

# Solusi

AS5600 dibaca:

```text
berkala menggunakan millis()
```

---

# Sebelum

```text
loop:
  baca I2C terus
```

---

# Sesudah

```text
loop:
  slave.poll()

  tiap 50 ms:
      baca AS5600
```

---

# Kenapa Berhasil?

Karena:

```text
Modbus diprioritaskan
```

dan:

```text
I2C tidak memonopoli CPU
```

---

# Wire Timeout

```cpp
Wire.setWireTimeout(3000, true);
```

Digunakan agar:

```text
I2C tidak freeze selamanya
```

jika terjadi:

* bus stuck
* SDA short
* sensor error

---

# Kenapa delay() Tidak Dipakai?

Karena:

```text
delay()
```

akan:

```text
menghentikan loop
```

Akibatnya:

```text
slave.poll()
```

tidak berjalan.

---

# millis()

Digunakan untuk:

```text
non-blocking timing
```

---

# Kenapa Non-Blocking Penting?

Karena embedded system:

```text
harus menangani banyak task bersamaan
```

contoh:

* sensor
* Modbus
* serial
* motor
* encoder

---

# Insight Penting

Masalah terbesar embedded system
sering kali bukan:

* sensor
* wiring
* library

tetapi:

* timing
* polling
* blocking code
* CPU scheduling
* serial latency

---

# Kesimpulan

Tahap ini berhasil membuktikan bahwa:

* AS5600 dapat dibaca tanpa library khusus
* data encoder dapat dikirim lewat Modbus RTU
* I2C dan Modbus dapat berjalan bersamaan
* non-blocking architecture sangat penting

Tahap ini menjadi fondasi:

```text
distributed robotic communication system
```

```
```
