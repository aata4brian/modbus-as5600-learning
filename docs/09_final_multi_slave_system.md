# Final Multi-Slave Modbus System

---

# Overview

Tahap akhir repository ini menggunakan:

```text
1 Master
2 Slave
````

dengan komunikasi:

```text
Modbus RTU + RS485
```

---

# Arsitektur Sistem

```text
AS5600
   ↓ I2C
Arduino Slave 1
   ↓ RS485
Arduino Master
   ↕
Laptop Slave 2
```

---

# Fungsi Tiap Node

---

# Arduino Master

Tugas:

* membaca semua slave
* menggabungkan data
* pusat komunikasi sistem

Menggunakan:

* SoftwareSerial
* MAX485
* Modbus RTU Master

---

# Arduino Slave 1

Tugas:

* membaca encoder AS5600
* menyimpan data ke holding register
* mengirim data ke master

Menggunakan:

* Hardware UART
* MAX485
* AS5600
* Modbus RTU Slave

---

# Laptop Slave 2

Tugas:

* mengirim array data
* simulasi command
* simulasi target
* simulasi trajectory

Menggunakan:

* Python
* pymodbus
* USB to RS485

---

# Kenapa Multi-Slave Penting?

Karena robot modern biasanya:

```text
distributed system
```

---

# Distributed System

Artinya:

```text
beberapa node bekerja bersama
```

contoh:

* sensor node
* motor node
* navigation node
* communication node

---

# Keuntungan Distributed System

* modular
* mudah dikembangkan
* mudah debugging
* scalable
* lebih rapi

---

# Topologi RS485

Semua device berada pada:

```text
bus yang sama
```

---

# Wiring Bus

```text
MASTER A -------- SLAVE1 A -------- USB RS485 A
MASTER B -------- SLAVE1 B -------- USB RS485 B

GND semua disatukan
```

---

# Kenapa RS485 Cocok?

Karena RS485 mendukung:

* multi-drop
* industrial communication
* long distance
* noise immunity

---

# Slave ID

| Device      | Slave ID |
| ----------- | -------- |
| AS5600 Node | 1        |
| Laptop Node | 2        |

---

# Register Map Slave 1

| Register | Fungsi        |
| -------- | ------------- |
| 0        | encoder angle |
| 1        | heartbeat     |
| 2        | sensor status |

---

# Register Map Slave 2

| Register | Fungsi    |
| -------- | --------- |
| 0        | data0     |
| 1        | data1     |
| 2        | data2     |
| 3        | data3     |
| 4        | heartbeat |

---

# Kenapa Menggunakan Register Map?

Karena:

```text
register map = bahasa komunikasi antar node
```

---

# Naming Convention

Project ini mulai menggunakan:

```text
naming convention yang lebih rapi
```

---

# Contoh

## Jangan

```cpp
regs
data
x
y
temp
```

---

# Lebih Baik

```cpp
encoderAngle
slaveHeartbeat
holdingRegs
modbusRequestTimer
```

---

# Kenapa Naming Penting?

Karena project embedded:

```text
akan berkembang sangat cepat
```

dan:

```text
naming buruk = debugging sulit
```

---

# Master Program Overview

Master melakukan:

```text
request → receive → print
```

secara bergantian ke:

* slave 1
* slave 2

---

# State Machine

Master menggunakan:

```text
state machine sederhana
```

---

# Kenapa?

Karena komunikasi serial:

```text
asynchronous
```

---

# State

| State | Fungsi                  |
| ----- | ----------------------- |
| 0     | request slave 1         |
| 1     | tunggu response slave 1 |
| 2     | request slave 2         |
| 3     | tunggu response slave 2 |

---

# Kenapa Tidak Langsung Semua?

Karena:

```text
Modbus RTU half duplex
```

hanya bisa:

```text
1 komunikasi pada satu waktu
```

---

# Half Duplex

Artinya:

```text
tidak bisa transmit dan receive bersamaan
```

---

# MAX485 Direction Control

MAX485 membutuhkan:

```text
direction control
```

menggunakan:

* RE
* DE

---

# Kenapa Laptop Lebih Mudah?

Karena:

```text
USB to RS485 converter
```

biasanya sudah memiliki:

```text
auto direction control
```

---

# Jadi Laptop Hanya Perlu

```text
A
B
(optional GND)
```

---

# Kenapa Slave AS5600 Menggunakan Hardware UART?

Karena slave:

* membaca I2C
* menjalankan Modbus
* real-time polling

lebih berat dibanding master.

---

# Kenapa Master Masih Pakai SoftwareSerial?

Karena master:

```text
lebih ringan
```

Tugasnya hanya:

* request
* receive
* print

---

# Masalah Besar yang Pernah Terjadi

Saat SDA AS5600 disambungkan:

```text
Modbus langsung fail
Serial freeze
```

---

# Penyebab Utama

Bukan:

* sensor rusak
* wiring salah
* RS485 rusak

melainkan:

```text
timing CPU overload
```

---

# Kenapa?

Karena:

* I2C polling terlalu sering
* SoftwareSerial sensitif timing
* slave.poll() terlambat

---

# Solusi

Menggunakan:

```text
non-blocking architecture
```

---

# Non-Blocking Loop

```cpp
loop:

  poll modbus

  jika waktunya:
      baca sensor

  jika waktunya:
      print debug
```

---

# Kenapa Ini Penting?

Karena embedded system:

```text
multitasking secara pseudo-parallel
```

---

# Pseudo Parallel

Walaupun Arduino:

```text
single core
```

tetapi dengan:

```text
millis()
```

sistem terasa:

```text
seperti multitasking
```

---

# Insight Tentang Embedded System

Masalah terbesar embedded system biasanya bukan:

* sensor
* wiring
* library

melainkan:

* timing
* polling
* latency
* blocking code
* serial timing
* interrupt scheduling

---

# Kenapa Ini Penting Untuk Robotik?

Karena robot:

```text
harus menangani banyak task bersamaan
```

contoh:

* membaca sensor
* komunikasi
* motor control
* encoder
* navigation
* telemetry

---

# Future Expansion

Arsitektur ini bisa dikembangkan menjadi:

* motor driver node
* IMU node
* navigation node
* CAN Bus system
* ESP32 distributed system

---

# Rekomendasi Pengembangan Selanjutnya

## Software

* PID control
* odometry
* robot kinematics
* RTOS
* DMA UART
* CAN Bus

---

# Hardware

* ESP32
* STM32
* industrial RS485 transceiver
* isolated RS485
* BLDC driver

---

# Main Insight

Project ini membuktikan bahwa:

* komunikasi industrial dapat dipelajari dengan Arduino
* Modbus RTU sangat cocok untuk distributed system
* timing adalah faktor paling penting
* non-blocking architecture sangat penting
* naming dan struktur code sangat berpengaruh

---

# Final Conclusion

Repository ini bukan hanya tentang:

* Modbus
* AS5600
* RS485

tetapi tentang:

```text
cara berpikir embedded system
```

yang meliputi:

* komunikasi
* timing
* architecture
* scalability
* distributed system design

```
```
