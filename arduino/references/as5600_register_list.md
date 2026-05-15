# AS5600 Register List

| Register | Nama | Fungsi |
|---|---|---|
| 0x00 | ZMCO | jumlah burn ZPOS/MPOS |
| 0x01 | ZPOS High | zero position high byte |
| 0x02 | ZPOS Low | zero position low byte |
| 0x03 | MPOS High | maximum position high byte |
| 0x04 | MPOS Low | maximum position low byte |
| 0x05 | MANG High | maximum angle high byte |
| 0x06 | MANG Low | maximum angle low byte |
| 0x07 | CONF High | configuration high byte |
| 0x08 | CONF Low | configuration low byte |
| 0x0B | STATUS | status magnet |
| 0x0C | RAW ANGLE High | raw angle high |
| 0x0D | RAW ANGLE Low | raw angle low |
| 0x0E | ANGLE High | processed angle high |
| 0x0F | ANGLE Low | processed angle low |
| 0x1A | MAGNITUDE High | magnetic strength high |
| 0x1B | MAGNITUDE Low | magnetic strength low |
| 0x1C | AGC | automatic gain control |

---

# Register Penting

## 0x0E dan 0x0F

Digunakan untuk membaca angle encoder.

---

## 0x0B

Digunakan untuk membaca status magnet.

---

## 0x07 dan 0x08

Digunakan untuk konfigurasi AS5600.

---

# STATUS Register

| Bit | Nama | Fungsi |
|---|---|---|
| bit 5 | MD | magnet detected |
| bit 4 | ML | magnet terlalu lemah |
| bit 3 | MH | magnet terlalu kuat |

---

# Contoh

## 0x20

Binary:

```text
0010 0000
````

Artinya:

```text
bit 5 aktif
```

---

## 0x10

Binary:

```text
0001 0000
```

Artinya:

```text
bit 4 aktif
```

---

## 0x08

Binary:

```text
0000 1000
```

Artinya:

```text
bit 3 aktif
```

---

# Angle Range

AS5600 menggunakan:

```text
12-bit resolution
```

Range:

```text
0 - 4095
```

mewakili:

```text
0° - 360°
```

---

# Degree Formula

```cpp
float degree =
(angle * 360.0)
/
4096.0;
```

---

# Wraparound Problem

Karena AS5600 single-turn:

```text
4095 → 0
```

maka perlu:

* wraparound correction
* multi-turn tracking

untuk aplikasi robotik dan odometry.

```
```
