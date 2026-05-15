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
// HOLDING REGISTER
// regs[0] = data dari Python
// regs[1] = heartbeat
// ======================================
uint16_t regs[2] = {0, 0};

void setup() {

  Serial.begin(115200);

  modbusPort.begin(9600);

  modbusPort.listen();

  slave.start();

  Serial.println(
    "MODBUS SLAVE READY"
  );
}

void loop() {

  // ======================================
  // MODBUS POLLING
  // ======================================
  slave.poll(regs, 2);

  // ======================================
  // HEARTBEAT
  // ======================================
  regs[1]++;

  // ======================================
  // DEBUG SERIAL
  // ======================================
  static unsigned long lastPrint = 0;

  if (
    millis() - lastPrint
    >= 500
  ) {

    lastPrint = millis();

    Serial.print("DATA: ");

    Serial.print(regs[0]);

    Serial.print(" | HB: ");

    Serial.println(regs[1]);
  }
}
```

---

```python
# python/01_python_modbus_master/python_master_write_register.py

from pymodbus.client.sync import ModbusSerialClient
import time

# ==========================================
# MODBUS MASTER
# ==========================================
client = ModbusSerialClient(

    method='rtu',

    port='COM23',

    baudrate=9600,

    timeout=1
)

# ==========================================
# CONNECT
# ==========================================
if not client.connect():

    print("Gagal konek")

    exit()

print("Modbus connected")

# ==========================================
# MAIN LOOP
# ==========================================
while True:

    # ======================================
    # WRITE REGISTER 0
    # VALUE = 123
    # SLAVE ID = 1
    # ======================================
    client.write_register(

        0,

        123,

        unit=1
    )

    print("Kirim 123")

    time.sleep(1)

    # ======================================
    # WRITE REGISTER 0
    # VALUE = 999
    # ======================================
    client.write_register(

        0,

        999,

        unit=1
    )

    print("Kirim 999")

    time.sleep(1)
```

---

````md
# references/modbus_function_codes.md

# Modbus Function Codes

---

# Overview

Function code menentukan:
```text
aksi apa yang diminta master
````

dalam komunikasi Modbus.

---

# Function Code Yang Dipakai Repository Ini

| Function Code | Nama                   | Fungsi                   |
| ------------- | ---------------------- | ------------------------ |
| 3             | Read Holding Registers | membaca register slave   |
| 6             | Write Single Register  | menulis 1 register slave |

---

# Function Code 3

```text
Read Holding Registers
```

Digunakan untuk:

```text
membaca data dari slave
```

---

# Contoh

Master meminta:

```text
slave ID 1
register 0
jumlah 3 register
```

Slave membalas:

```text
[1234, 55, 1]
```

---

# Pada Library Arduino

```cpp
telegram.u8fct = 3;
```

---

# Function Code 6

```text
Write Single Register
```

Digunakan untuk:

```text
menulis satu register slave
```

---

# Contoh

Master menulis:

```text
register 0 = 123
```

pada:

```text
slave ID 1
```

---

# Pada Python

```python
client.write_register(
    0,
    123,
    unit=1
)
```

---

# Pada Arduino

```cpp
telegram.u8fct = 6;
```

---

# Kenapa Function Code Penting?

Karena function code menentukan:

* jenis operasi
* format data
* arah komunikasi
* response slave

---

# Function Code Lain Yang Umum

| Code | Nama                     |
| ---- | ------------------------ |
| 1    | Read Coils               |
| 2    | Read Discrete Inputs     |
| 4    | Read Input Registers     |
| 5    | Write Single Coil        |
| 15   | Write Multiple Coils     |
| 16   | Write Multiple Registers |

---

# Kenapa Repository Ini Fokus ke 3 dan 6?

Karena:

* paling sederhana
* paling umum
* cukup untuk embedded learning
* cocok untuk sensor dan robotik

---

# Insight Penting

Dalam Modbus:

```text
function code adalah inti komunikasi
```

karena menentukan:

* siapa membaca
* siapa menulis
* format packet
* struktur response

```
```
