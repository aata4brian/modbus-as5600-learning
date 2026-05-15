# RS485 Basic

---

# Apa itu RS485?

RS485 adalah standar komunikasi serial differential
yang sangat umum digunakan pada:

- industrial automation
- PLC
- robot industri
- CNC
- sensor network
- Modbus RTU

Repository ini menggunakan:
```text
RS485 + Modbus RTU
````

---

# Kenapa Menggunakan RS485?

Karena RS485:

* tahan noise
* bisa multi-device
* jarak komunikasi jauh
* stabil untuk industrial environment

---

# RS485 vs UART Biasa

## UART TTL biasa

Menggunakan:

```text
TX
RX
GND
```

Biasanya:

```text
point-to-point
```

---

# Kekurangan UART TTL

* jarak pendek
* mudah kena noise
* tidak cocok multi-device
* tidak industrial grade

---

# RS485

Menggunakan:

```text
A
B
```

dan optional:

```text
GND
```

---

# Kelebihan RS485

* differential signal
* noise immunity tinggi
* multi-drop bus
* jarak jauh
* industrial standard

---

# Differential Signal

RS485 menggunakan:

```text
perbedaan tegangan
```

antara:

```text
A dan B
```

bukan terhadap ground.

---

# Kenapa Differential Penting?

Karena noise biasanya mengenai:

```text
A dan B bersamaan
```

maka:

```text
selisih tegangan tetap sama
```

Akibatnya:

```text
lebih tahan gangguan
```

---

# Multi-Drop Bus

RS485 memungkinkan:

```text
banyak device
```

pada:

```text
1 jalur bus
```

---

# Contoh Topologi

```text
MASTER A ---- SLAVE1 A ---- SLAVE2 A
MASTER B ---- SLAVE1 B ---- SLAVE2 B
```

---

# Semua Device Paralel

RS485:

```text
BUKAN point-to-point
```

tetapi:

```text
shared bus
```

---

# Half Duplex

Repository ini menggunakan:

```text
half duplex RS485
```

Artinya:

```text
tidak bisa transmit dan receive bersamaan
```

---

# Kenapa Half Duplex?

Karena:

```text
jalur komunikasi dipakai bersama
```

---

# Konsekuensi Half Duplex

Harus ada:

```text
direction control
```

untuk menentukan:

```text
TX mode
RX mode
```

---

# MAX485

Repository ini menggunakan:

```text
MAX485
```

---

# Fungsi MAX485

MAX485 adalah:

```text
UART ↔ RS485 transceiver
```

---

# Tugas MAX485

Mengubah:

```text
UART TTL
```

menjadi:

```text
RS485 differential signal
```

---

# Pin MAX485

| Pin | Fungsi          |
| --- | --------------- |
| DI  | Data Input      |
| RO  | Receiver Output |
| DE  | Driver Enable   |
| RE  | Receiver Enable |
| A   | RS485 A         |
| B   | RS485 B         |

---

# Wiring Arduino ↔ MAX485

| Arduino    | MAX485  |
| ---------- | ------- |
| TX         | DI      |
| RX         | RO      |
| pin enable | RE + DE |

---

# RE dan DE

MAX485 membutuhkan:

```text
direction control
```

---

# DE

```text
Driver Enable
```

Mengaktifkan:

```text
transmit
```

---

# RE

```text
Receiver Enable
```

Mengaktifkan:

```text
receive
```

---

# Biasanya Digabung

Dalam project ini:

```text
RE dan DE disatukan
```

---

# Mode MAX485

| State | Mode     |
| ----- | -------- |
| LOW   | Receive  |
| HIGH  | Transmit |

---

# Kenapa Perlu Direction Control?

Karena RS485:

```text
half duplex
```

---

# Kalau Salah Direction?

Maka:

* data tidak terkirim
* data collision
* bus conflict
* Modbus timeout

---

# USB to RS485

Laptop menggunakan:

```text
USB to RS485 converter
```

---

# Kenapa Lebih Mudah?

Karena converter USB RS485 biasanya sudah memiliki:

```text
auto direction control
```

---

# Jadi Laptop Cukup

```text
A
B
(optional GND)
```

---

# Tidak Perlu

```text
RO
DI
RE
DE
```

karena:

```text
sudah diurus internal converter
```

---

# GND pada RS485

Walaupun RS485 differential:

```text
GND tetap disarankan common
```

---

# Kenapa?

Karena device tetap membutuhkan:

```text
common voltage reference
```

---

# Wiring RS485 Final

```text
MASTER A -------- SLAVE1 A -------- USB RS485 A
MASTER B -------- SLAVE1 B -------- USB RS485 B

GND semua disatukan
```

---

# Terminasi RS485

Pada sistem panjang biasanya digunakan:

```text
120 ohm termination resistor
```

di:

```text
ujung bus
```

---

# Untuk Apa?

Untuk:

* mengurangi reflection
* memperbaiki signal integrity
* meningkatkan stabilitas komunikasi

---

# Untuk Prototype Pendek

Biasanya:

```text
belum wajib
```

---

# Baudrate

Repository ini menggunakan:

```text
9600 baud
```

karena:

* stabil
* aman untuk SoftwareSerial
* cukup untuk pembelajaran

---

# Semakin Tinggi Baudrate

Semakin:

* sensitif timing
* sensitif noise
* sensitif interrupt

---

# RS485 dan Embedded System

RS485 sangat umum digunakan karena:

* murah
* sederhana
* stabil
* industrial proven

---

# Penggunaan RS485

Banyak digunakan pada:

* Modbus RTU
* industrial PLC
* VFD
* CNC
* motor driver
* robot industri
* sensor network

---

# Insight Penting

Masalah RS485 sering kali bukan:

```text
wiring saja
```

tetapi:

* timing
* direction control
* polling
* grounding
* blocking code
* SoftwareSerial limitation

adalah faktor yang sangat penting.

```
```
