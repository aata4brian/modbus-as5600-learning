# Python to Arduino Modbus RTU

---

# Overview

Pada tahap ini:

```text
Python = Modbus Master
Arduino = Modbus Slave
````

Komunikasi menggunakan:

* Modbus RTU
* RS485
* MAX485
* USB to RS485

---

# Tujuan Pembelajaran

Mempelajari:

* dasar Modbus RTU
* komunikasi Python ↔ Arduino
* holding register
* write register
* RS485 communication
* Modbus master-slave architecture

---

# Arsitektur Sistem

```text
Python Laptop
     ↓
USB to RS485
     ↓
RS485 Bus
     ↓
MAX485
     ↓
Arduino Slave
```

---

# Hardware

* Arduino UNO / Nano
* MAX485
* USB to RS485 converter
* Jumper wires
* Laptop

---

# Wiring Arduino ↔ MAX485

| Arduino | MAX485  |
| ------- | ------- |
| pin 11  | DI      |
| pin 10  | RO      |
| pin 4   | RE + DE |
| 5V      | VCC     |
| GND     | GND     |

---

# Wiring RS485

| MAX485 | USB RS485 |
| ------ | --------- |
| A      | A         |
| B      | B         |

optional:

```text
GND ↔ GND
```

---

# Kenapa Menggunakan SoftwareSerial?

Karena:

```text
pin 0 dan 1
```

digunakan oleh:

```text
USB Serial Monitor
```

maka Modbus dipindahkan ke:

```text
pin 10 dan 11
```

---

# Arduino Slave Program

```cpp
#include <SoftwareSerial.h>
#include <ModbusRtu.h>

// ======================================
// MODBUS PIN
// ======================================
#define MODBUS_RX 10
#define MODBUS_TX 11
#define RS485_EN  4

// ======================================
// SOFTWARE SERIAL
// ======================================
SoftwareSerial modbusPort(
  MODBUS_RX,
  MODBUS_TX
);

// ======================================
// MODBUS SLAVE
// slave id = 1
// ======================================
Modbus slave(1, modbusPort, RS485_EN);

// ======================================
// REGISTER
// regs[0] = data dari Python
// regs[1] = heartbeat
// ======================================
uint16_t regs[2] = {0, 0};

void setup() {

  Serial.begin(115200);

  modbusPort.begin(9600);

  modbusPort.listen();

  slave.start();

  Serial.println("MODBUS SLAVE READY");
}

void loop() {

  // ======================================
  // MODBUS POLL
  // ======================================
  slave.poll(regs, 2);

  // ======================================
  // HEARTBEAT
  // ======================================
  regs[1]++;

  // ======================================
  // SERIAL MONITOR
  // ======================================
  static unsigned long t = 0;

  if (millis() - t > 500) {

    t = millis();

    Serial.print("DATA: ");
    Serial.print(regs[0]);

    Serial.print(" | HB: ");
    Serial.println(regs[1]);
  }
}
```

---

# Penjelasan Program Arduino

---

# SoftwareSerial

```cpp
SoftwareSerial modbusPort(
  MODBUS_RX,
  MODBUS_TX
);
```

Membuat serial baru pada:

* pin 10 = RX
* pin 11 = TX

---

# Modbus Slave

```cpp
Modbus slave(1, modbusPort, RS485_EN);
```

Artinya:

* slave ID = 1
* menggunakan modbusPort
* pin enable MAX485 = pin 4

---

# Holding Register

```cpp
uint16_t regs[2] = {0, 0};
```

Register map:

| Register | Fungsi           |
| -------- | ---------------- |
| regs[0]  | data dari Python |
| regs[1]  | heartbeat        |

---

# slave.poll()

```cpp
slave.poll(regs, 2);
```

Fungsi:

* menerima request master
* memproses Modbus
* update holding register
* mengirim response

---

# Heartbeat

```cpp
regs[1]++;
```

Digunakan untuk memastikan:

```text
Arduino masih hidup
```

---

# Python Program

Install library:

```bash
pip install pymodbus==2.5.3 pyserial
```

---

# Python Master Program

```python
from pymodbus.client.sync import ModbusSerialClient
import time

client = ModbusSerialClient(
    method='rtu',
    port='COM23',
    baudrate=9600,
    timeout=1
)

if not client.connect():
    print("Gagal konek")
    exit()

print("Modbus connected")

while True:

    client.write_register(
        0,
        123,
        unit=1
    )

    print("Kirim 123")

    time.sleep(1)

    client.write_register(
        0,
        999,
        unit=1
    )

    print("Kirim 999")

    time.sleep(1)
```

---

# Penjelasan Python

---

# ModbusSerialClient

```python
client = ModbusSerialClient(...)
```

Membuat:

```text
Modbus RTU master
```

---

# Parameter Penting

| Parameter     | Fungsi                 |
| ------------- | ---------------------- |
| method='rtu'  | menggunakan Modbus RTU |
| port='COM23'  | port USB RS485         |
| baudrate=9600 | baudrate serial        |
| timeout=1     | timeout 1 detik        |

---

# write_register()

```python
client.write_register(
    0,
    123,
    unit=1
)
```

Artinya:

* tulis register 0
* nilai = 123
* slave ID = 1

---

# Function Code 6

write_register menggunakan:

```text
Function Code 6
```

yaitu:

```text
Write Single Register
```

---

# Alur Komunikasi

## Python

```text
tulis register 0 = 123
```

↓

## Arduino

```text
regs[0] menjadi 123
```

↓

## Serial Monitor

```text
DATA: 123
```

---

# Output Python

```text
Modbus connected
Kirim 123
Kirim 999
```

---

# Output Arduino

```text
DATA: 123 | HB: 1234
DATA: 999 | HB: 2345
```

---

# Kenapa Menggunakan RS485?

Karena RS485:

* lebih stabil
* tahan noise
* industrial standard
* bisa multi-device

---

# Masalah Umum

## Tidak Connect

Penyebab:

* COM port salah
* USB RS485 belum terdeteksi

---

# Tidak Ada Data

Penyebab:

* A/B tertukar
* slave ID salah
* baudrate berbeda

---

# Timeout

Penyebab:

* slave.poll() tidak dipanggil
* wiring salah
* direction control salah

---

# Kenapa delay() Tidak Dipakai di Arduino?

Karena:

```text
slave.poll()
```

harus dipanggil sesering mungkin.

Kalau loop terlalu lambat:

```text
Modbus timeout
```

---

# Insight Penting

Dalam Modbus RTU:

```text
timing sangat penting
```

Masalah terbesar biasanya bukan:

* library
* sensor

melainkan:

* polling
* timeout
* serial timing
* direction control
* blocking code

adalah faktor yang sangat penting.

```
```
