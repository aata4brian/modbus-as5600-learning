# SoftwareSerial vs Hardware UART

---

# Overview

Dalam repository ini digunakan dua jenis serial:

- SoftwareSerial
- Hardware UART

Perbedaan keduanya sangat penting
dalam embedded system dan komunikasi real-time.

---

# UART

UART adalah:
```text
Universal Asynchronous Receiver Transmitter
````

Digunakan untuk:

* komunikasi serial
* TX/RX
* Modbus RTU
* komunikasi device

---

# Hardware UART

Hardware UART adalah:

```text
peripheral UART asli
```

yang ada di dalam microcontroller.

---

# Contoh Hardware UART

Pada Arduino UNO:

| Pin | Fungsi |
| --- | ------ |
| 0   | RX     |
| 1   | TX     |

Digunakan oleh:

```text
Serial
```

---

# Kenapa Disebut Hardware UART?

Karena:

```text
komunikasi serial ditangani hardware khusus
```

bukan software biasa.

---

# Hardware UART Memiliki

* baudrate generator
* shift register
* hardware buffer
* interrupt hardware

---

# Keuntungan Hardware UART

* stabil
* cepat
* akurat
* tidak membebani CPU terlalu besar
* cocok untuk real-time communication

---

# Hardware UART Sangat Cocok Untuk

* Modbus RTU
* robotik
* sensor real-time
* komunikasi industrial
* multi-device communication

---

# SoftwareSerial

SoftwareSerial adalah:

```text
UART buatan software
```

---

# Kenapa Dibuat?

Karena Arduino UNO hanya punya:

```text
1 hardware UART
```

padahal kadang dibutuhkan:

```text
lebih dari 1 serial
```

---

# Contoh

```cpp
SoftwareSerial modbusPort(10, 11);
```

Artinya:

```text
pin 10 = RX
pin 11 = TX
```

---

# Cara Kerja SoftwareSerial

SoftwareSerial menggunakan:

```text
bit-banging
```

---

# Apa itu Bit-Banging?

Artinya:

```text
CPU sendiri yang membaca timing bit
```

secara manual.

---

# Jadi CPU Harus

* menghitung timing bit
* membaca bit
* mengirim bit
* sinkronisasi serial

semua dilakukan oleh:

```text
software
```

---

# Konsekuensi SoftwareSerial

Karena CPU menangani serial secara manual:

```text
SoftwareSerial lebih sensitif
```

terhadap:

* interrupt
* delay
* timing
* blocking code
* sensor polling

---

# Timing Serial

Pada:

```text
9600 baud
```

1 bit ≈:

```text
104 us
```

---

# Artinya

CPU harus:

```text
membaca bit tepat waktu
```

---

# Kalau CPU Sibuk

Misalnya:

* I2C
* delay()
* servo
* interrupt
* sensor polling

maka:

```text
timing bit bisa miss
```

---

# Akibatnya

* data corrupt
* CRC error
* Modbus timeout
* framing error

---

# Semakin Tinggi Baudrate

Semakin:

```text
sedikit waktu CPU
```

untuk membaca bit.

---

# Contoh

## 9600 baud

1 bit:

```text
≈ 104 us
```

---

## 115200 baud

1 bit:

```text
≈ 8.68 us
```

---

# Artinya

Pada 115200:

```text
CPU hanya punya 8 mikrodetik
```

untuk membaca bit berikutnya.

---

# SoftwareSerial dan Modbus RTU

Modbus RTU sangat sensitif terhadap:

* timing
* latency
* packet spacing

---

# Maka

SoftwareSerial:

```text
lebih mudah timeout
```

dibanding hardware UART.

---

# Masalah yang Terjadi di Project Ini

Awalnya:

```text
AS5600 dibaca terus-menerus
```

sementara:

```text
Modbus menggunakan SoftwareSerial
```

---

# Akibatnya

CPU sibuk:

* I2C polling
* Wire.requestFrom()
* Wire.endTransmission()

dan:

```text
slave.poll()
```

jarang dipanggil.

---

# Dampaknya

Master melihat:

```text
TIMEOUT
NO RESPONSE
MODBUS FAIL
```

padahal:

```text
wiring benar
```

---

# Penyebab Utama

Bukan:

* RS485 rusak
* AS5600 rusak
* Modbus library rusak

melainkan:

```text
timing CPU overload
```

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
  baca I2C
  baca I2C
  baca I2C
  baca I2C
```

---

# Sesudah

```text
loop:
  slave.poll()

  tiap 50ms:
      baca AS5600
```

---

# Hasilnya

CPU punya waktu untuk:

```text
memproses Modbus
```

dan sistem menjadi stabil.

---

# Kenapa Slave Dipindah ke Hardware UART?

Karena slave:

* membaca sensor
* menjalankan Modbus
* melakukan polling
* lebih berat

---

# Maka

Slave menggunakan:

```text
Hardware UART
```

agar:

* lebih stabil
* lebih real-time
* lebih tahan timing issue

---

# Kenapa Master Masih Pakai SoftwareSerial?

Karena master:

```text
lebih ringan
```

Tugas master hanya:

* request
* receive
* print

jadi:

```text
masih aman menggunakan SoftwareSerial
```

---

# Rule of Thumb Embedded System

## Gunakan Hardware UART Untuk

* Modbus RTU
* komunikasi penting
* sensor real-time
* robotik
* komunikasi multi-node

---

# Gunakan SoftwareSerial Untuk

* debug tambahan
* device sederhana
* baudrate rendah
* komunikasi non-critical

---

# Baudrate Aman SoftwareSerial

| Baudrate | Status           |
| -------- | ---------------- |
| 4800     | sangat aman      |
| 9600     | aman             |
| 19200    | mulai sensitif   |
| 38400    | kadang error     |
| 57600    | sering error     |
| 115200   | tidak disarankan |

---

# Faktor yang Membuat SoftwareSerial Error

* interrupt terlalu banyak
* blocking code
* I2C polling berat
* delay panjang
* servo library
* NeoPixel
* sensor terlalu banyak
* baudrate terlalu tinggi

---

# Insight Penting

Dalam embedded system:

```text
timing adalah segalanya
```

Masalah terbesar sering kali bukan:

* wiring
* sensor
* library

tetapi:

* latency
* blocking loop
* interrupt timing
* CPU scheduling
* serial timing

adalah faktor yang sangat penting.

```
```
