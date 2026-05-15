# Pin Mapping

---

# Arduino Master

| Pin | Fungsi |
|---|---|
| 10 | Modbus RX |
| 11 | Modbus TX |
| 4 | RS485 RE + DE |
| USB | Serial Monitor |

---

# Arduino Slave 1

| Pin | Fungsi |
|---|---|
| 0 | Hardware RX |
| 1 | Hardware TX |
| 4 | RS485 RE + DE |
| A4 | AS5600 SDA |
| A5 | AS5600 SCL |

---

# MAX485

| Pin | Fungsi |
|---|---|
| DI | UART TX input |
| RO | UART RX output |
| DE | driver enable |
| RE | receiver enable |
| A | RS485 A |
| B | RS485 B |

---

# AS5600

| Pin | Fungsi |
|---|---|
| SDA | I2C data |
| SCL | I2C clock |
| VCC | power |
| GND | ground |

---

# USB RS485

| Pin | Fungsi |
|---|---|
| A | RS485 A |
| B | RS485 B |
| GND | optional common ground |

---

# Final Bus Wiring

```text
MASTER A -------- SLAVE1 A -------- USB RS485 A
MASTER B -------- SLAVE1 B -------- USB RS485 B

GND semua disatukan
