# Arduino to Arduino Modbus RTU

---

# Overview

Pada tahap ini:

```text
Arduino 1 = Modbus Master
Arduino 2 = Modbus Slave
````

Komunikasi menggunakan:

* Modbus RTU
* RS485
* MAX485
* SoftwareSerial
* Holding Register

---

# Tujuan Pembelajaran

Mempelajari:

* komunikasi Arduino ke Arduino
* master-slave Modbus
* write single register
* holding register
* RS485 bus
* debugging lewat Serial Monitor

---

# Arsitektur Sistem

```text
Arduino Master
     ↓
MAX485
     ↓ RS485
MAX485
     ↓
Arduino Slave
```

---

# Hardware

* 2x Arduino UNO / Nano
* 2x MAX485
* Jumper wires
* Breadboard

---

# Wiring Master ke MAX485

| Arduino Master | MAX485  |
| -------------- | ------- |
| pin 11         | DI      |
| pin 10         | RO      |
| pin 4          | RE + DE |
| 5V             | VCC     |
| GND            | GND     |

---

# Wiring Slave ke MAX485

| Arduino Slave | MAX485  |
| ------------- | ------- |
| pin 11        | DI      |
| pin 10        | RO      |
| pin 4         | RE + DE |
| 5V            | VCC     |
| GND           | GND     |

---

# Wiring RS485 Bus

| Master MAX485 | Slave MAX485 |
| ------------- | ------------ |
| A             | A            |
| B             | B            |
| GND           | GND          |

Jika tidak jalan:

```text
coba tukar A dan B
```

---

# Slave ID

Pada contoh ini:

```text
Slave ID = 1
```

---

# Register Map Slave

| Register | Fungsi           |
| -------- | ---------------- |
| regs[0]  | data dari master |
| regs[1]  | heartbeat        |

---

# Arduino Slave Program

```cpp
#include <SoftwareSerial.h>
#include <ModbusRtu.h>

#define MODBUS_RX 10
#define MODBUS_TX 11
#define RS485_EN  4

SoftwareSerial modbusPort(MODBUS_RX, MODBUS_TX);

// Slave ID = 1
Modbus slave(1, modbusPort, RS485_EN);

// regs[0] = data dari master
// regs[1] = heartbeat
uint16_t regs[2] = {0, 0};

void setup() {
  Serial.begin(115200);

  modbusPort.begin(9600);
  modbusPort.listen();

  slave.start();

  Serial.println("SLAVE READY");
}

void loop() {
  slave.poll(regs, 2);

  regs[1]++;

  static unsigned long t = 0;
  if (millis() - t > 500) {
    t = millis();

    Serial.print("SLAVE DATA: ");
    Serial.print(regs[0]);

    Serial.print(" | HB: ");
    Serial.println(regs[1]);
  }
}
```

---

# Penjelasan Slave Program

---

# SoftwareSerial

```cpp
SoftwareSerial modbusPort(MODBUS_RX, MODBUS_TX);
```

Membuat serial baru untuk Modbus.

| Pin | Fungsi |
| --- | ------ |
| 10  | RX     |
| 11  | TX     |

---

# Modbus Slave

```cpp
Modbus slave(1, modbusPort, RS485_EN);
```

Artinya:

* device ini adalah slave
* slave ID = 1
* komunikasi lewat modbusPort
* pin 4 mengontrol MAX485

---

# slave.poll()

```cpp
slave.poll(regs, 2);
```

Fungsi:

* mendengar request dari master
* memproses frame Modbus
* menulis / membaca holding register
* mengirim response

---

# Heartbeat

```cpp
regs[1]++;
```

Heartbeat digunakan untuk mengetahui bahwa slave masih aktif.

Jika master membaca heartbeat dan nilainya berubah,
berarti komunikasi dan slave masih hidup.

---

# Arduino Master Program

```cpp
#include <SoftwareSerial.h>
#include <ModbusRtu.h>

#define MODBUS_RX 10
#define MODBUS_TX 11
#define RS485_EN  4

SoftwareSerial modbusPort(MODBUS_RX, MODBUS_TX);

Modbus master(0, modbusPort, RS485_EN);
modbus_t telegram;

// regs[0] = data yang dikirim ke slave
uint16_t regs[1] = {0};

uint16_t value = 0;
unsigned long timer = 0;

void sendToSlave(uint16_t data) {
  regs[0] = data;

  telegram.u8id = 1;          // slave ID
  telegram.u8fct = 6;         // write single register
  telegram.u16RegAdd = 0;     // register 0
  telegram.u16CoilsNo = 1;
  telegram.au16reg = regs;

  master.query(telegram);
}

void setup() {
  Serial.begin(115200);

  modbusPort.begin(9600);
  modbusPort.listen();

  master.start();
  master.setTimeOut(300);

  Serial.println("MASTER READY");
}

void loop() {
  master.poll();

  if (millis() - timer >= 1000) {
    timer = millis();

    value += 10;

    if (value > 1000) {
      value = 0;
    }

    sendToSlave(value);

    Serial.print("MASTER SEND: ");
    Serial.println(value);
  }
}
```

---

# Penjelasan Master Program

---

# Modbus Master

```cpp
Modbus master(0, modbusPort, RS485_EN);
```

Dalam library ini:

| ID    | Fungsi |
| ----- | ------ |
| 0     | Master |
| 1-247 | Slave  |

---

# Telegram

```cpp
modbus_t telegram;
```

Telegram adalah paket request Modbus.

Berisi:

* slave ID tujuan
* function code
* alamat register
* jumlah register
* pointer data

---

# Function Code 6

```cpp
telegram.u8fct = 6;
```

Artinya:

```text
Write Single Register
```

Master menulis satu register ke slave.

---

# Register Address

```cpp
telegram.u16RegAdd = 0;
```

Artinya:

```text
tulis ke register 0
```

---

# Data Register

```cpp
telegram.au16reg = regs;
```

Artinya data yang dikirim berada di array:

```cpp
regs
```

---

# master.query()

```cpp
master.query(telegram);
```

Mengirim request Modbus ke slave.

---

# master.poll()

```cpp
master.poll();
```

Memproses komunikasi Modbus master.

Walaupun master hanya menulis data,
poll tetap perlu dipanggil agar library dapat memproses state komunikasi.

---

# Alur Komunikasi

```text
Master:
  value = 10
  write register 0 ke slave

Slave:
  regs[0] berubah menjadi 10
  print ke Serial Monitor
```

---

# Output Master

```text
MASTER READY
MASTER SEND: 10
MASTER SEND: 20
MASTER SEND: 30
```

---

# Output Slave

```text
SLAVE READY
SLAVE DATA: 10 | HB: 1234
SLAVE DATA: 20 | HB: 2345
SLAVE DATA: 30 | HB: 3456
```

---

# Catatan Timeout

Pada beberapa percobaan, master bisa menampilkan timeout,
tetapi data tetap masuk ke slave.

Hal ini bisa terjadi karena:

* SoftwareSerial timing kurang stabil
* response ack dari slave tidak selalu terbaca
* Modbus write berhasil tetapi master gagal membaca response
* direction control MAX485 terlalu cepat berubah

---

# Kenapa Data Tetap Masuk?

Karena request dari master sampai ke slave,
sehingga slave berhasil menulis:

```cpp
regs[0]
```

Tetapi balasan dari slave ke master tidak selalu terbaca dengan benar.

---

# Solusi Sementara

Untuk tahap belajar:

```text
fokus lihat data di slave
```

Jika slave menerima data, berarti komunikasi master ke slave sudah berhasil.

---

# Solusi Lebih Stabil

Gunakan:

* hardware UART
* Arduino Mega
* ESP32 Serial2
* baudrate lebih rendah
* wiring lebih pendek
* grounding lebih baik

---

# Insight Penting

Komunikasi Modbus bukan hanya soal:

* query
* register
* function code

tetapi juga:

* timing
* UART stability
* RS485 direction control
* SoftwareSerial limitation
* polling loop

---

# Kesimpulan

Tahap ini berhasil membuktikan bahwa:

* Arduino dapat menjadi Modbus master
* Arduino lain dapat menjadi Modbus slave
* data dapat dikirim lewat RS485
* holding register dapat berubah sesuai request master

Ini adalah dasar untuk membangun sistem multi-node robotik.

```
```
