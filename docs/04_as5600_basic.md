# AS5600 Basic

---

# Apa itu AS5600?

AS5600 adalah:
```text
12-bit magnetic rotary encoder
````

yang digunakan untuk membaca:

* sudut rotasi
* posisi shaft
* posisi motor
* wheel rotation

---

# Cara Kerja AS5600

AS5600 membaca:

```text
medan magnet
```

dari:

```text
magnet permanen
```

yang dipasang pada shaft.

---

# Output AS5600

AS5600 menghasilkan:

```text
nilai sudut absolut
```

Range:

```text
0 - 4095
```

karena:

```text
12-bit resolution
```

---

# Kenapa 4095?

Karena:

```text
2^12 = 4096
```

dan counting dimulai dari:

```text
0
```

maka:

```text
0 - 4095
```

---

# Absolute Encoder

AS5600 adalah:

```text
absolute encoder
```

---

# Apa Artinya?

Saat power ON:

```text
sensor langsung tahu posisi sudut
```

tanpa perlu:

```text
homing
```

---

# Berbeda Dengan Incremental Encoder

Incremental encoder:

```text
harus menghitung pulse
```

sedangkan AS5600:

```text
langsung memberi posisi absolut
```

---

# Interface AS5600

AS5600 mendukung:

* I2C
* analog output
* PWM output

Repository ini menggunakan:

```text
I2C
```

---

# Wiring AS5600

| AS5600 | Arduino   |
| ------ | --------- |
| SDA    | A4        |
| SCL    | A5        |
| VCC    | 3.3V / 5V |
| GND    | GND       |

---

# I2C

I2C adalah:

```text
Inter-Integrated Circuit
```

protokol komunikasi:

```text
2-wire serial bus
```

---

# Jalur I2C

| Pin | Fungsi |
| --- | ------ |
| SDA | data   |
| SCL | clock  |

---

# Pada Arduino UNO

| Pin | Fungsi |
| --- | ------ |
| A4  | SDA    |
| A5  | SCL    |

---

# Alamat I2C AS5600

AS5600 menggunakan:

```cpp
0x36
```

---

# Register AS5600

AS5600 memiliki banyak register.

Repository ini fokus pada:

```text
register angle
```

---

# Register Angle

| Register | Fungsi          |
| -------- | --------------- |
| 0x0E     | ANGLE high byte |
| 0x0F     | ANGLE low byte  |

---

# Membaca Register

Contoh:

```cpp
Wire.beginTransmission(0x36);

Wire.write(0x0E);

Wire.endTransmission(false);

Wire.requestFrom(0x36, 2);
```

---

# Penjelasan

---

# beginTransmission()

Memulai komunikasi I2C.

---

# Wire.write()

Menentukan:

```text
register yang ingin dibaca
```

---

# endTransmission(false)

Mengirim register address tanpa:

```text
melepaskan bus I2C
```

---

# requestFrom()

Meminta:

```text
2 byte data
```

dari AS5600.

---

# High Byte dan Low Byte

AS5600 mengirim:

* high byte
* low byte

---

# Kenapa 2 Byte?

Karena:

```text
12-bit data
```

tidak muat dalam:

```text
1 byte
```

---

# 1 Byte

1 byte:

```text
8-bit
```

Range:

```text
0 - 255
```

---

# 2 Byte

2 byte:

```text
16-bit
```

Range:

```text
0 - 65535
```

---

# Penggabungan Byte

```cpp
uint16_t angle =
((uint16_t)highByte << 8)
| lowByte;
```

---

# Penjelasan

## highByte << 8

Menggeser:

```text
high byte ke atas
```

---

# Operator <<

```text
left shift
```

contoh:

```text
00000001
```

shift 1:

```text
00000010
```

---

# Operator |

```text
bitwise OR
```

Digunakan untuk:

```text
menggabungkan bit
```

---

# Masking 12-bit

```cpp
angle &= 0x0FFF;
```

---

# Kenapa Perlu Masking?

Karena:

```text
AS5600 hanya memakai 12-bit
```

---

# 0x0FFF

Binary:

```text
0000 1111 1111 1111
```

---

# Artinya

Hanya:

```text
12 bit bawah
```

yang dipakai.

---

# Konversi Sudut

Range:

```text
0 - 4095
```

mewakili:

```text
0° - 360°
```

---

# Rumus Derajat

```cpp
float degree =
(angle * 360.0) / 4096.0;
```

---

# Kenapa Dibagi 4096?

Karena:

```text
12-bit = 4096 step
```

---

# Multi-Turn Problem

AS5600 adalah:

```text
single-turn encoder
```

---

# Artinya

Setelah:

```text
4095
```

akan kembali ke:

```text
0
```

---

# Contoh

```text
4090
4092
4094
2
4
6
```

---

# Masalah

Kalau dihitung biasa:

```text
2 - 4094 = -4092
```

padahal sebenarnya:

```text
bergerak maju sedikit
```

---

# Solusi Wraparound Correction

Contoh:

```cpp
if (diff > 2048) {
  diff -= 4096;
}
else if (diff < -2048) {
  diff += 4096;
}
```

---

# Kenapa 2048?

Karena:

```text
setengah dari 4096
```

---

# Tujuannya

Menentukan:

```text
arah rotasi paling logis
```

---

# Contoh

## Kasus Forward Wrap

```text
4094 → 2
```

Normal:

```text
2 - 4094 = -4092
```

dikoreksi:

```text
-4092 + 4096 = 4
```

---

# Hasil

Sekarang sistem tahu:

```text
rotasi maju kecil
```

---

# Pull-Up I2C

I2C membutuhkan:

```text
pull-up resistor
```

pada:

* SDA
* SCL

---

# Biasanya

```text
4.7kΩ
```

ke:

```text
3.3V atau 5V
```

---

# Kenapa?

Karena I2C menggunakan:

```text
open-drain communication
```

---

# Open Drain

Device hanya bisa:

* menarik LOW
* melepas jalur

HIGH dibuat oleh:

```text
pull-up resistor
```

---

# Masalah yang Pernah Terjadi

Saat SDA disambungkan:

```text
Serial Monitor freeze
Modbus fail
```

---

# Penyebab

Bukan:

* wiring salah
* AS5600 rusak

melainkan:

```text
I2C polling terlalu sering
```

dan:

```text
SoftwareSerial kehilangan timing
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
  baca I2C terus-menerus
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

# Hasil

* Modbus stabil
* AS5600 tetap terbaca
* SoftwareSerial tidak timeout

---

# Penggunaan AS5600

AS5600 sering digunakan pada:

* robot wheel encoder
* steering encoder
* BLDC control
* position sensing
* odometry
* actuator feedback

---

# Kelebihan AS5600

* murah
* presisi tinggi
* mudah digunakan
* tidak contact mechanical
* absolute encoder

---

# Kekurangan AS5600

* hanya single-turn
* sensitif posisi magnet
* tidak sekuat industrial encoder mahal

---

# Insight Penting

Masalah terbesar saat menggunakan AS5600
sering kali bukan:

* sensor
* wiring

melainkan:

* timing
* polling
* blocking I2C
* SoftwareSerial limitation

adalah faktor yang sangat penting.

```
```
