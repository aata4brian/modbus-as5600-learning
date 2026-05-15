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
