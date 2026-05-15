# Modbus RTU Basic

---

# Apa itu Modbus?

Modbus adalah protokol komunikasi industrial
yang digunakan untuk pertukaran data antar device.

Contoh:
- PLC
- Arduino
- Sensor
- Motor driver
- HMI
- Industrial controller

---

# Jenis Modbus

## 1. Modbus RTU

Menggunakan:
- UART Serial
- RS232
- RS485

Format data:
- binary

Keunggulan:
- cepat
- ringan
- sederhana

Repository ini menggunakan:
```text
Modbus RTU
````

---

## 2. Modbus TCP

Menggunakan:

* Ethernet
* TCP/IP

Biasanya digunakan pada:

* industrial network
* SCADA
* factory automation

---

# Arsitektur Modbus

Modbus menggunakan:

```text
Master - Slave
```

---

# Master

Master bertugas:

* meminta data
* mengontrol komunikasi
* menentukan siapa yang berbicara

Dalam project ini:

```text
Arduino Master
```

---

# Slave

Slave bertugas:

* menunggu request
* menjawab request master

Dalam project ini:

```text
Arduino AS5600 Node
Laptop Python Node
```

---

# Penting

Dalam Modbus:

```text
slave tidak boleh kirim data duluan
```

Slave:

```text
hanya menjawab saat diminta master
```

---

# Topologi Komunikasi

```text
Master
   ↓
Slave 1
Slave 2
Slave 3
```

Semua slave berada pada:

```text
bus yang sama
```

---

# Slave ID

Setiap slave memiliki:

```text
alamat unik
```

Contoh:

| Device      | Slave ID |
| ----------- | -------- |
| AS5600 Node | 1        |
| Laptop Node | 2        |

---

# Function Code

Function code menentukan:

```text
aksi apa yang diminta master
```

---

# Function Code yang Dipakai

## Function Code 3

```text
Read Holding Registers
```

Digunakan untuk:

```text
membaca register slave
```

---

## Function Code 6

```text
Write Single Register
```

Digunakan untuk:

```text
menulis 1 register slave
```

---

# Holding Register

Holding register adalah:

```text
tempat penyimpanan data Modbus
```

Biasanya:

```cpp
uint16_t holdingRegs[10];
```

---

# Kenapa uint16_t?

Karena register Modbus:

```text
16-bit
```

Range:

```text
0 - 65535
```

---

# Contoh Register Map

| Register | Isi           |
| -------- | ------------- |
| 0        | encoder angle |
| 1        | heartbeat     |
| 2        | sensor status |

---

# Contoh Pembacaan

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

Artinya:

```text
angle = 1234
heartbeat = 55
status = OK
```

---

# RS485 dalam Modbus RTU

Repository ini menggunakan:

```text
RS485
```

karena:

* bisa multi-device
* tahan noise
* jarak lebih jauh
* industrial standard

---

# Kenapa Tidak UART Biasa?

UART biasa:

```text
point-to-point
```

RS485:

```text
multi-drop bus
```

---

# Multi-Drop Bus

Semua device berada di:

```text
jalur A dan B yang sama
```

Contoh:

```text
MASTER A ---- SLAVE1 A ---- SLAVE2 A
MASTER B ---- SLAVE1 B ---- SLAVE2 B
```

---

# Timing dalam Modbus RTU

Modbus RTU sangat sensitif terhadap:

* timing
* latency
* blocking code

Karena komunikasi serial:

```text
real-time
```

---

# Polling

Master harus:

```text
poll slave secara berkala
```

Contoh:

```cpp
master.poll();
```

---

# Kenapa Polling Penting?

Karena:

```text
master.poll()
```

memproses:

* request
* response
* CRC
* timeout
* parsing packet

---

# Timeout

Master memiliki:

```text
timeout
```

Contoh:

```cpp
master.setTimeOut(300);
```

Artinya:

```text
jika slave tidak menjawab dalam 300ms
anggap gagal
```

---

# CRC

Modbus RTU menggunakan:

```text
CRC checksum
```

untuk memastikan:

```text
data tidak corrupt
```

---

# Kenapa Modbus Banyak Dipakai?

Karena:

* sederhana
* stabil
* ringan
* open standard
* mudah diimplementasi

---

# Kelebihan Modbus RTU

* mudah dipelajari
* hemat resource
* cocok untuk embedded system
* cocok untuk industrial system

---

# Kekurangan Modbus RTU

* half duplex
* timing sensitif
* master-slave only
* bandwidth terbatas

---

# Penggunaan Umum

Modbus RTU banyak dipakai pada:

* PLC
* VFD
* motor driver
* CNC
* robot industri
* automation system

---

# Insight Penting

Dalam embedded system:

```text
komunikasi serial bukan hanya soal wiring
```

tetapi:

* timing
* polling
* latency
* scheduling
* non-blocking loop

adalah faktor yang sangat penting.

```
```
