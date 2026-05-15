# AS5600 Register Map

---

# Overview

AS5600 memiliki banyak register internal
yang digunakan untuk:

- konfigurasi sensor
- membaca sudut
- status magnet
- output mode
- filtering
- watchdog
- zero position

Repository ini fokus pada:
- pembacaan angle
- konfigurasi dasar
- understanding register system

---

# I2C Address

AS5600 menggunakan alamat:

```cpp
0x36
````

---

# Format Register

Sebagian besar register AS5600:

```text
8-bit
```

tetapi beberapa data:

```text
16-bit
```

yang terdiri dari:

* high byte
* low byte

---

# Register List

| Register | Nama           | Fungsi                     |
| -------- | -------------- | -------------------------- |
| 0x00     | ZMCO           | jumlah burn ZPOS/MPOS      |
| 0x01     | ZPOS High      | zero position high byte    |
| 0x02     | ZPOS Low       | zero position low byte     |
| 0x03     | MPOS High      | maximum position high byte |
| 0x04     | MPOS Low       | maximum position low byte  |
| 0x05     | MANG High      | maximum angle high byte    |
| 0x06     | MANG Low       | maximum angle low byte     |
| 0x07     | CONF High      | configuration high byte    |
| 0x08     | CONF Low       | configuration low byte     |
| 0x0B     | STATUS         | status magnet              |
| 0x0C     | RAW ANGLE High | raw angle high             |
| 0x0D     | RAW ANGLE Low  | raw angle low              |
| 0x0E     | ANGLE High     | processed angle high       |
| 0x0F     | ANGLE Low      | processed angle low        |
| 0x1A     | MAGNITUDE High | magnetic strength high     |
| 0x1B     | MAGNITUDE Low  | magnetic strength low      |
| 0x1C     | AGC            | automatic gain control     |

---

# 0x0E dan 0x0F

Register paling penting.

Digunakan untuk:

```text
membaca sudut encoder
```

---

# ANGLE Register

| Register | Fungsi          |
| -------- | --------------- |
| 0x0E     | angle high byte |
| 0x0F     | angle low byte  |

---

# Range

Nilai:

```text
0 - 4095
```

mewakili:

```text
0° - 360°
```

---

# Cara Membaca

```cpp
Wire.beginTransmission(0x36);

Wire.write(0x0E);

Wire.endTransmission(false);

Wire.requestFrom(0x36, 2);
```

---

# RAW ANGLE vs ANGLE

## RAW ANGLE

Register:

```text
0x0C dan 0x0D
```

berisi:

```text
sudut mentah sensor
```

tanpa:

* filtering
* scaling
* zero correction

---

## ANGLE

Register:

```text
0x0E dan 0x0F
```

berisi:

```text
hasil angle yang sudah diproses
```

biasanya lebih stabil.

---

# STATUS Register

Register:

```cpp
0x0B
```

---

# Fungsi

Memberi informasi:

```text
kondisi magnet
```

---

# Bit STATUS

| Bit   | Nama | Fungsi               |
| ----- | ---- | -------------------- |
| bit 5 | MD   | magnet detected      |
| bit 4 | ML   | magnet terlalu lemah |
| bit 3 | MH   | magnet terlalu kuat  |

---

# Apa itu Bit?

Bit adalah:

```text
unit data paling kecil
```

Nilai:

```text
0 atau 1
```

---

# Contoh

Hex:

```text
0x20
```

Binary:

```text
0010 0000
```

Artinya:

```text
bit 5 = 1
```

---

# Kenapa 0x20 = bit 5?

Karena:

| Bit   | Nilai |
| ----- | ----- |
| bit 0 | 1     |
| bit 1 | 2     |
| bit 2 | 4     |
| bit 3 | 8     |
| bit 4 | 16    |
| bit 5 | 32    |

dan:

```text
32 decimal = 0x20
```

---

# Contoh Cek Bit

```cpp
if (status & 0x20)
```

Artinya:

```text
cek apakah bit 5 aktif
```

---

# Operator &

```text
bitwise AND
```

Digunakan untuk:

```text
mengambil bit tertentu
```

---

# Contoh

```text
status = 0010 1000
mask   = 0010 0000
```

hasil:

```text
0010 0000
```

berarti:

```text
bit 5 aktif
```

---

# AGC Register

Register:

```text
0x1C
```

---

# Fungsi

AGC:

```text
Automatic Gain Control
```

Digunakan untuk:

```text
menyesuaikan sensitivitas sensor
```

---

# MAGNITUDE Register

| Register | Fungsi    |
| -------- | --------- |
| 0x1A     | high byte |
| 0x1B     | low byte  |

---

# Fungsi

Menunjukkan:

```text
kekuatan medan magnet
```

---

# Kenapa Penting?

Karena AS5600 sangat bergantung pada:

```text
posisi magnet
```

---

# Jika Magnet Bermasalah

Maka:

* angle tidak stabil
* noise tinggi
* pembacaan salah

---

# CONF Register

| Register | Fungsi    |
| -------- | --------- |
| 0x07     | CONF High |
| 0x08     | CONF Low  |

---

# Fungsi CONF

Mengatur:

* filtering
* watchdog
* hysteresis
* output mode
* PWM mode
* slow filter
* fast filter

---

# Menulis CONF

Contoh:

```cpp
Wire.beginTransmission(0x36);

Wire.write(0x07);

Wire.write(highByte);

Wire.write(lowByte);

Wire.endTransmission();
```

---

# Burn

AS5600 memiliki:

```text
burn feature
```

---

# Apa itu Burn?

Burn:

```text
menyimpan konfigurasi permanen
```

ke:

```text
internal OTP memory
```

---

# OTP

OTP:

```text
One Time Programmable
```

Artinya:

```text
hanya bisa ditulis beberapa kali
```

---

# Hati-Hati Burn

Karena:

```text
tidak bisa diundo
```

---

# Biasanya Burn Dipakai Untuk

* zero position
* maximum angle
* final calibration

---

# Zero Position

ZPOS:

```text
titik nol encoder
```

---

# Kenapa Penting?

Karena kadang:

```text
magnet tidak dipasang pas
```

---

# Maka

Posisi nol bisa:

```text
digeser lewat register
```

---

# Maximum Position

MPOS:

```text
batas maksimum rotasi
```

---

# Maximum Angle

MANG:

```text
sudut maksimum kerja
```

---

# Contoh Penggunaan

Kalau hanya ingin:

```text
0° - 180°
```

maka:

```text
range bisa dibatasi
```

---

# Filtering

AS5600 memiliki:

* slow filter
* fast filter

---

# Tujuan Filtering

Untuk:

* mengurangi noise
* membuat output stabil

---

# Konsekuensi Filtering

Semakin besar filter:

```text
lebih stabil
```

tetapi:

```text
lebih lambat respon
```

---

# Watchdog

Watchdog digunakan untuk:

```text
monitor internal sensor
```

---

# Hysteresis

Hysteresis digunakan untuk:

```text
mengurangi jitter kecil
```

---

# Jitter

Jitter:

```text
perubahan kecil yang tidak stabil
```

contoh:

```text
1000
1001
999
1000
1002
```

---

# Konversi Hex

Contoh:

```text
0x0B
```

---

# Cara Membaca

## 0x

Artinya:

```text
hexadecimal
```

---

# B

Hex:

```text
A = 10
B = 11
C = 12
D = 13
E = 14
F = 15
```

---

# Jadi

```text
0x0B = 11 decimal
```

---

# Binary dari 0x0B

```text
0000 1011
```

---

# Insight Penting

AS5600 sebenarnya:

```text
cukup kompleks
```

dan banyak fitur:

* filtering
* calibration
* angle scaling
* magnetic monitoring

yang sering tidak dipakai
pada tutorial sederhana.

---

# Kesimpulan

Register paling penting dalam repository ini:

| Register | Fungsi             |
| -------- | ------------------ |
| 0x0E     | angle high         |
| 0x0F     | angle low          |
| 0x0B     | magnet status      |
| 0x07     | configuration high |
| 0x08     | configuration low  |

---

# Main Insight

Masalah terbesar saat membaca AS5600
biasanya bukan:

* rumus angle
* wiring

tetapi:

* I2C timing
* polling rate
* magnet placement
* filtering
* SoftwareSerial timing

adalah faktor yang sangat penting.

```
```
