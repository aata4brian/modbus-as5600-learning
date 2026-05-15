#include <SoftwareSerial.h>
#include <ModbusRtu.h>

// ======================================
// MODBUS PIN
// ======================================
#define MODBUS_RX 10
#define MODBUS_TX 11
#define RS485_EN  4

// ======================================
// SOFTWARE SERIAL
// ======================================
SoftwareSerial modbusPort(
  MODBUS_RX,
  MODBUS_TX
);

// ======================================
// MODBUS MASTER
// ID 0 = master
// ======================================
Modbus master(
  0,
  modbusPort,
  RS485_EN
);

modbus_t telegram;

// ======================================
// DATA REGISTER
// regs[0] = data yang dikirim ke slave
// ======================================
uint16_t regs[1] = {0};

uint16_t value = 0;
unsigned long timer = 0;

// ======================================
// SEND DATA TO SLAVE
// ======================================
void sendToSlave(uint16_t data) {

  regs[0] = data;

  telegram.u8id = 1;          // slave ID
  telegram.u8fct = 6;         // write single register
  telegram.u16RegAdd = 0;     // register address
  telegram.u16CoilsNo = 1;    // jumlah register
  telegram.au16reg = regs;    // data pointer

  master.query(telegram);
}

void setup() {

  Serial.begin(115200);

  modbusPort.begin(9600);

  modbusPort.listen();

  master.start();

  master.setTimeOut(300);

  Serial.println("MASTER READY");
}

void loop() {

  // wajib dipanggil agar state Modbus berjalan
  master.poll();

  if (millis() - timer >= 1000) {

    timer = millis();

    value += 10;

    if (value > 1000) {
      value = 0;
    }

    sendToSlave(value);

    Serial.print("MASTER SEND: ");
    Serial.println(value);
  }
}
```

```cpp
// arduino/03_arduino_to_arduino_slave/arduino_slave_receive_register.ino

#include <SoftwareSerial.h>
#include <ModbusRtu.h>

// ======================================
// MODBUS PIN
// ======================================
#define MODBUS_RX 10
#define MODBUS_TX 11
#define RS485_EN  4

// ======================================
// SOFTWARE SERIAL
// ======================================
SoftwareSerial modbusPort(
  MODBUS_RX,
  MODBUS_TX
);

// ======================================
// MODBUS SLAVE
// slave ID = 1
// ======================================
Modbus slave(
  1,
  modbusPort,
  RS485_EN
);

// ======================================
// HOLDING REGISTER
// regs[0] = data dari master
// regs[1] = heartbeat
// ======================================
uint16_t regs[2] = {0, 0};

unsigned long lastPrint = 0;

void setup() {

  Serial.begin(115200);

  modbusPort.begin(9600);

  modbusPort.listen();

  slave.start();

  Serial.println("SLAVE READY");
}

void loop() {

  // wajib dipanggil sesering mungkin
  slave.poll(regs, 2);

  regs[1]++;

  if (millis() - lastPrint >= 500) {

    lastPrint = millis();

    Serial.print("SLAVE DATA: ");
    Serial.print(regs[0]);

    Serial.print(" | HB: ");
    Serial.println(regs[1]);
  }
}
```

````md
# references/software_serial_vs_hardware_uart.md

# SoftwareSerial vs Hardware UART

## Ringkasan

SoftwareSerial adalah serial yang dibuat lewat software.

Hardware UART adalah serial yang memang sudah disediakan oleh hardware microcontroller.

---

## Hardware UART

Contoh pada Arduino UNO:

- RX = pin 0
- TX = pin 1

Dipakai oleh:

```cpp
Serial
````

Hardware UART memiliki:

* baudrate generator
* shift register
* buffer hardware
* interrupt hardware

Karena itu hardware UART lebih stabil untuk komunikasi real-time.

---

## SoftwareSerial

Contoh:

```cpp
SoftwareSerial modbusPort(10, 11);
```

Artinya:

* pin 10 = RX
* pin 11 = TX

SoftwareSerial bekerja dengan cara bit-banging.

Artinya CPU sendiri yang membaca dan mengirim bit serial secara manual.

---

## Kenapa SoftwareSerial Lebih Sensitif?

Karena pada SoftwareSerial, CPU harus membaca bit tepat waktu.

Pada 9600 baud:

```text
1 bit ≈ 104 microsecond
```

Kalau CPU sedang sibuk menjalankan I2C, delay, print serial, atau sensor polling, timing bit bisa terlewat.

Akibatnya:

* data corrupt
* CRC error
* Modbus timeout
* response gagal terbaca

---

## Rule of Thumb

| Baudrate | SoftwareSerial   |
| -------- | ---------------- |
| 4800     | sangat aman      |
| 9600     | aman             |
| 19200    | mulai sensitif   |
| 38400    | kadang error     |
| 57600    | sering error     |
| 115200   | tidak disarankan |

---

## Kenapa Slave AS5600 Lebih Baik Pakai Hardware UART?

Karena slave AS5600 melakukan dua hal:

1. membaca AS5600 lewat I2C
2. melayani Modbus RTU

Kalau Modbus juga pakai SoftwareSerial, CPU harus menangani:

* I2C
* Modbus
* SoftwareSerial timing

secara bergantian.

Ini bisa menyebabkan timeout.

---

## Kesimpulan

SoftwareSerial cocok untuk eksperimen sederhana.

Hardware UART lebih cocok untuk sistem robotik yang serius dan stabil.

```
```
