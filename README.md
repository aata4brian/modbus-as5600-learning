# Modbus RTU + AS5600 Embedded Learning

Repository ini berisi pembelajaran dan eksperimen mengenai:

- Modbus RTU
- RS485
- Arduino Master-Slave
- Python Modbus
- AS5600 Magnetic Encoder
- Embedded System Architecture
- Timing dan Non-Blocking System
- SoftwareSerial vs Hardware UART

Repository ini dibuat sebagai knowledge base pembelajaran embedded system dan robotic communication system.

---

# Project Architecture

AS5600
   ↓ I2C
Arduino Slave 1
   ↓ RS485 Modbus RTU
Arduino Master
   ↕
Laptop Slave 2 (Python Modbus)

---

# System Overview

## Arduino Master

Tugas:

* membaca semua slave
* menggabungkan data
* menjadi pusat komunikasi

Menggunakan:

* SoftwareSerial
* MAX485
* Modbus RTU Master

---

## Arduino Slave 1

Tugas:

* membaca encoder AS5600
* menyimpan data ke holding register
* mengirim data ke master

Menggunakan:

* Hardware UART
* MAX485
* AS5600
* Modbus RTU Slave

---

## Laptop Slave 2

Tugas:

* mengirim array data
* simulasi target
* simulasi command
* simulasi trajectory

Menggunakan:

* Python
* pymodbus
* USB to RS485

---

# Learning Roadmap

## 1. Python to Arduino Modbus

* Python sebagai Modbus master
* Arduino sebagai slave
* komunikasi RS485 dasar

---

## 2. Arduino to Arduino Modbus

* Arduino master
* Arduino slave
* read/write holding register

---

## 3. AS5600 Basic

* komunikasi I2C
* membaca register
* 12-bit encoder

---

## 4. AS5600 + Modbus

* slave membaca encoder
* data dikirim lewat Modbus RTU

---

## 5. Multi-Slave Modbus System

* Slave 1 = AS5600 Node
* Slave 2 = Laptop Node
* Arduino sebagai master utama

---

# Hardware Used

* Arduino UNO / Nano
* MAX485
* USB to RS485
* AS5600
* Breadboard
* Jumper wires

---

# Communication Protocol

## RS485

RS485 menggunakan:

* A
* B

dan optional:

* GND common reference

RS485 hanya menangani komunikasi data,
bukan supply daya.

---

## Modbus RTU

Repository ini menggunakan:

* Holding Register
* Function Code 3
* Function Code 6

---

# SoftwareSerial vs Hardware UART

## SoftwareSerial

Kelebihan:

* fleksibel
* bisa memakai pin lain

Kekurangan:

* sensitif timing
* CPU intensive
* kurang stabil untuk real-time

---

## Hardware UART

Kelebihan:

* stabil
* hardware assisted
* lebih cocok untuk Modbus RTU

Digunakan pada:

* slave encoder AS5600

---

# Important Embedded System Concepts

Repository ini fokus pada:

* timing
* blocking vs non-blocking
* polling
* communication latency
* distributed system
* resource sharing

---

# Main Insight

Masalah terbesar dalam embedded system biasanya bukan wiring,
melainkan:

* timing
* blocking code
* SoftwareSerial limitation
* interrupt handling
* communication latency

---

# Repository Structure

docs/
hardware/
arduino/
python/
references/
images/

---

# Current Progress

* [x] Python ↔ Arduino Modbus
* [x] Arduino ↔ Arduino Modbus
* [x] AS5600 basic reader
* [x] AS5600 + Modbus
* [x] Multi-slave Modbus system

---

# Future Goals

* Multi-turn encoder
* PID control
* Odometry
* Robot kinematics
* CAN Bus
* ESP32 migration
* RTOS
* DMA UART
* Motion control system

---

# Repository Purpose

Repository ini dibuat untuk:

* dokumentasi pembelajaran
* eksperimen embedded system
* referensi robotik
* latihan arsitektur komunikasi industri
* knowledge base pribadi
