# Register, packet and timing contract

Source audit: encoder sketch 06, master sketch 07 and the Python slave example. All Modbus addresses below are **zero-based protocol offsets**, not 40001-style human labels.

| Slave | Offset | Value | Meaning |
| --- | ---: | --- | --- |
| 1 | 0 | 0–4095 | AS5600 angle register, masked to 12 bits |
| 1 | 1 | uint16 | Acquisition heartbeat; wraps at 65535 |
| 1 | 2 | 0 or 1 | I2C read failed/succeeded |
| 2 | 0–3 | uint16 | Synthetic counter values, offsets 0, 10, 20, 30 |
| 2 | 4 | uint16 | Laptop heartbeat |

Angle in degrees is `raw * 360 / 4096`. This is one-turn angle; continuous multi-turn position and wheel odometry are not implemented. The slave reads I2C address `0x36`, starting at `0x0E`. It retains the last angle on read failure and clears status.

The final master reads three holding registers from slave 1 and five from slave 2 using function code `0x03`. Earlier lessons also use single-register writes (`0x06`). For FC03, a request contains address (1 byte), function (1), start offset (2), register count (2) and CRC (2); a normal response contains address, function, byte count, two bytes per register and CRC. Multi-byte register fields are high-byte first; CRC is low-byte first. No captured packet is supplied here.

| Setting | Configured value | Evidence boundary |
| --- | --- | --- |
| Serial | 9600 baud, 8N1 | Explicit Python settings and Arduino defaults |
| Encoder acquisition | 50 ms threshold | Nominal 20 updates/s; loop work adds delay |
| Master cycle | 500 ms threshold | Nominal upper target ~2 cycles/s; not a benchmark |
| Master timeout | 300 ms | Configured timeout, not measured timeout accuracy |
| Post-query delay | 10 ms | Blocking delay in each request helper |
| Laptop update | 0.5 s sleep | Scheduling and work affect actual period |

At 9600 baud and 8N1, ten bits per character gives approximately 1.042 ms/character. The Modbus RTU specification defines inter-frame silence in character times; measure actual gaps and library handling on the bench. An 8N1 example should not be assumed interoperable with equipment configured for parity or two stop bits.

## Wiring summary

| Role | Connection |
| --- | --- |
| Master software RX / TX | D10 ← RO; D11 → DI |
| Encoder slave hardware RX / TX | D0 ← RO; D1 → DI |
| Both Arduino nodes | D4 → tied DE and active-low RE |
| Encoder I2C | A4 SDA, A5 SCL on UNO/Nano-style boards |
| RS485 bus | A to A; B to B; suitable reference ground per hardware topology |

Use a bus/daisy-chain topology. Verify the transceiver module voltage, termination and biasing before power-on; labels and onboard resistors vary. Do not connect 5V logic directly to a 3.3V-only controller. Hardware UART on the encoder node shares programming/debug resources: avoid serial debug traffic on the Modbus port.

References: [Modbus specifications](https://www.modbus.org/modbus-specifications), plus the AS5600 register notes already in `arduino/references/`. Consult the exact module datasheet before wiring.
