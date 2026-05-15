# Wiring Final Multi-Slave System

---

# Overview

Sistem final terdiri dari:

```text
Arduino Master
Arduino Slave 1 (AS5600)
Laptop Slave 2
````

Komunikasi menggunakan:

```text
Modbus RTU + RS485
```

---

# Topologi Sistem

```text
                ┌────────────┐
                │  LAPTOP    │
                └─────┬──────┘
                      │ USB
                ┌─────┴──────┐
                │ USB RS485  │
                └─────┬──────┘
                      │
                A─────┼─────────────A
                B─────┼─────────────B
                      │
        ┌─────────────┴─────────────┐
        │                           │

┌───────┴────────┐        ┌─────────┴────────┐
│ MAX485 MASTER  │        │ MAX485 SLAVE 1  │
└───────┬────────┘        └─────────┬────────┘
        │                           │

┌───────┴────────┐        ┌─────────┴────────┐
│ Arduino MASTER │        │ Arduino SLAVE 1 │
└────────────────┘        └─────────┬────────┘
                                    │
                              ┌─────┴─────┐
                              │  AS5600   │
                              └───────────┘
```

---

# Hardware List

* 2x Arduino UNO / Nano
* 2x MAX485
* 1x USB to RS485
* 1x AS5600
* Jumper wires
* Breadboard

---

# Arduino Master Wiring

---

# Arduino ↔ MAX485

| Arduino Master | MAX485  |
| -------------- | ------- |
| pin 11         | DI      |
| pin 10         | RO      |
| pin 4          | RE + DE |
| 5V             | VCC     |
| GND            | GND     |

---

# Kenapa Pin 10 dan 11?

Karena master menggunakan:

```text
SoftwareSerial
```

---

# SoftwareSerial

| Pin | Fungsi |
| --- | ------ |
| 10  | RX     |
| 11  | TX     |

---

# Kenapa Tidak Menggunakan Pin 0 dan 1?

Karena:

```text
pin 0 dan 1 dipakai USB Serial Monitor
```

---

# Arduino Slave 1 Wiring

---

# Arduino ↔ MAX485

| Arduino Slave | MAX485  |
| ------------- | ------- |
| TX (pin 1)    | DI      |
| RX (pin 0)    | RO      |
| pin 4         | RE + DE |
| 5V            | VCC     |
| GND           | GND     |

---

# Kenapa Menggunakan Hardware UART?

Karena slave:

* membaca AS5600
* menjalankan Modbus
* lebih berat

Maka:

```text
hardware UART lebih stabil
```

---

# Penting

Karena slave memakai:

```text
pin 0 dan 1
```

maka:

```text
jangan colok USB saat running
```

karena USB juga memakai:

```text
hardware UART
```

dan dapat menyebabkan:

* conflict
* serial corruption
* Modbus fail

---

# AS5600 Wiring

| AS5600 | Arduino Slave |
| ------ | ------------- |
| SDA    | A4            |
| SCL    | A5            |
| VCC    | 3.3V / 5V     |
| GND    | GND           |

---

# I2C Pins Arduino UNO

| Pin | Fungsi |
| --- | ------ |
| A4  | SDA    |
| A5  | SCL    |

---

# RS485 Bus Wiring

---

# Jalur A dan B

| MASTER MAX485 | SLAVE MAX485 | USB RS485 |
| ------------- | ------------ | --------- |
| A             | A            | A         |
| B             | B            | B         |

---

# Semua Device Paralel

RS485 menggunakan:

```text
shared bus
```

bukan:

```text
point-to-point
```

---

# Ground RS485

Disarankan:

| Device | Device |
| ------ | ------ |
| GND    | GND    |

---

# Kenapa GND Tetap Penting?

Walaupun RS485 differential:

```text
device tetap membutuhkan common reference
```

---

# USB to RS485 Wiring

USB RS485 biasanya hanya membutuhkan:

| USB RS485 | Fungsi  |
| --------- | ------- |
| A         | RS485 A |
| B         | RS485 B |

optional:

| Pin | Fungsi        |
| --- | ------------- |
| GND | common ground |

---

# Kenapa Tidak Ada RO/DI/RE/DE?

Karena USB RS485 converter biasanya sudah memiliki:

```text
auto direction control
```

dan:

```text
UART internal
```

---

# Maka Laptop Cukup

```text
USB
A
B
```

---

# Power Wiring

---

# Arduino Master

Master mendapatkan daya dari:

```text
USB Laptop
```

---

# Arduino Slave

Slave mendapatkan daya dari:

| Master | Slave |
| ------ | ----- |
| 5V     | 5V    |
| GND    | GND   |

---

# Jadi Total Kabel Antar Arduino

| Master | Slave |
| ------ | ----- |
| 5V     | 5V    |
| GND    | GND   |
| A      | A     |
| B      | B     |

---

# Kenapa RS485 Tidak Menyuplai Power?

Karena RS485:

```text
hanya komunikasi data
```

A dan B:

```text
bukan jalur supply
```

---

# Supply Yang Aman

Untuk prototype:

```text
master boleh menyuplai slave
```

---

# Untuk Sistem Besar

Disarankan:

* power supply terpisah
* buck converter
* isolated supply

---

# Common Wiring Mistakes

---

# A dan B Terbalik

Gejala:

* timeout
* no response
* Modbus fail

Solusi:

```text
tukar A dan B
```

---

# GND Tidak Nyambung

Gejala:

* komunikasi tidak stabil
* random timeout

---

# USB Masih Menancap di Slave

Gejala:

* hardware UART conflict
* data corrupt

---

# SDA/SCL Salah

Gejala:

* AS5600 tidak terbaca
* Wire freeze

---

# I2C Polling Terlalu Cepat

Gejala:

* Modbus fail
* Serial freeze

Penyebab:

```text
SoftwareSerial kehilangan timing
```

---

# Solusi Stabil

* gunakan millis()
* prioritaskan Modbus polling
* jangan blocking loop
* gunakan hardware UART untuk slave berat

---

# Final Insight

Masalah wiring embedded system sering kali bukan:

* kabel putus
* sensor rusak

melainkan:

* grounding
* timing
* serial conflict
* direction control
* polling architecture

adalah faktor yang sangat penting.

```
```
