# Troubleshooting

---

# Modbus Timeout

## Gejala

- timeout
- no response
- Modbus fail

---

# Penyebab Umum

- A/B tertukar
- slave ID salah
- baudrate berbeda
- GND tidak common
- slave.poll() terlalu jarang
- SoftwareSerial timing gagal

---

# Solusi

- tukar A dan B
- cek slave ID
- gunakan baudrate 9600
- gunakan millis()
- prioritaskan Modbus polling

---

# Serial Freeze Saat SDA Dipasang

## Gejala

Saat SDA disambungkan:
- Modbus fail
- Serial Monitor berhenti

---

# Penyebab

CPU terlalu sibuk membaca I2C.

Akibatnya:
```text
SoftwareSerial kehilangan timing
````

---

# Solusi

Gunakan:

```cpp
millis()
```

dan:

```text
non-blocking architecture
```

---

# Jangan Membaca AS5600 Terus-Menerus

## Salah

```cpp
loop:
  read AS5600
  read AS5600
  read AS5600
```

---

# Benar

```cpp
loop:

  slave.poll()

  tiap 50ms:
      read AS5600
```

---

# Slave Tidak Merespon

## Penyebab

* slave belum start()
* slave ID salah
* MAX485 direction salah

---

# Solusi

Pastikan:

```cpp
slave.start();
```

dan:

```cpp
slave.poll();
```

dipanggil terus.

---

# Data Masuk Tapi Timeout

## Penyebab

Request berhasil masuk,
tetapi response slave gagal terbaca.

Biasanya karena:

* SoftwareSerial
* timing
* direction switching terlalu cepat

---

# Solusi

* gunakan hardware UART
* kurangi baudrate
* gunakan wiring pendek

---

# USB Masih Menancap di Slave Hardware UART

## Gejala

* data random
* serial conflict
* Modbus rusak

---

# Penyebab

Pin 0 dan 1 dipakai:

* USB serial
* MAX485

secara bersamaan.

---

# Solusi

Saat running:

```text
cabut USB slave
```

---

# AS5600 Tidak Terbaca

## Penyebab

* SDA/SCL salah
* magnet terlalu jauh
* magnet terlalu dekat
* I2C freeze

---

# Solusi

* cek wiring A4/A5
* cek magnet
* gunakan Wire timeout

---

# Magnet Status Error

## ML

```text
magnet terlalu lemah
```

---

# MH

```text
magnet terlalu kuat
```

---

# MD Tidak Aktif

```text
magnet tidak terdeteksi
```

---

# SoftwareSerial Sangat Sensitif

Terutama jika:

* baudrate tinggi
* banyak interrupt
* banyak I2C
* banyak print Serial

---

# Rekomendasi

## Untuk Prototype

SoftwareSerial masih cukup.

---

# Untuk Sistem Serius

Gunakan:

* hardware UART
* ESP32
* STM32
* CAN Bus

---

# Insight Penting

Masalah embedded system paling sering bukan:

* sensor rusak
* library rusak

melainkan:

* timing
* polling
* latency
* interrupt
* CPU scheduling

adalah faktor yang sangat penting.

```
```
