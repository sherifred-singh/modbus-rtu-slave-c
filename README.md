# 📡 Industrial Modbus-RTU Slave Protocol Stack in C

[![C/C++ Modbus CI](https://github.com/Sherifred123/modbus-rtu-slave-c/actions/workflows/ci.yml/badge.svg)](https://github.com/Sherifred123/modbus-rtu-slave-c/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A lightweight, deterministic **Modbus-RTU Slave Protocol Stack** written in pure C99 for **Microchip PIC18**, **STM32**, and **ESP32** industrial controllers communicating over **RS-485** / UART.

---

## ⚡ Supported Modbus Function Codes

- `0x03` — Read Holding Registers
- `0x04` — Read Input Registers
- `0x06` — Write Single Register
- `0x10` — Write Multiple Registers
- **Standard Exception Codes:** `0x01` (Illegal Function), `0x02` (Illegal Data Address), `0x03` (Illegal Data Value)

---

## 📐 Packet Structure & CRC16 Validation

```text
┌──────────────┬───────────────┬───────────────────┬──────────────────┬──────────────┐
│  Slave Addr  │ Function Code │   Data Payload    │      CRC Lo      │    CRC Hi    │
│   (1 Byte)   │   (1 Byte)    │    (N-4 Bytes)    │     (1 Byte)     │   (1 Byte)   │
└──────────────┴───────────────┴───────────────────┴──────────────────┴──────────────┘
```

### Engineering Highlights:
- **Fast Table-Driven CRC-16:** 256-entry lookup table for sub-microsecond packet validation.
- **Zero Dynamic Memory Allocation:** Fixed static buffers for MISRA-C compliance.
- **Independent Hardware Layer:** Designed for seamless integration with hardware timers (3.5T character silent interval framing) and non-blocking UART FIFO ring buffers.

---

## 🧪 Running Unit Tests

```bash
make
```

---

## 📜 License
Released under the [MIT License](LICENSE). Designed and maintained by **Sherifred Singh**.
