# Industrial Modbus-RTU Slave Protocol Stack in C

A lightweight, deterministic **Modbus-RTU Slave Protocol Stack** written in pure C99 for **Microchip PIC18**, **STM32**, and **ESP32** industrial controllers over **RS-485**.

---

## Supported Modbus Function Codes

- `0x03` (Read Holding Registers)
- `0x04` (Read Input Registers)
- `0x06` (Write Single Register)
- `0x10` (Write Multiple Registers)
- Standard Exception Codes: `0x01` (Illegal Function), `0x02` (Illegal Data Address), `0x03` (Illegal Data Value)

---

## Engineering Features

- **Fast Table-Driven CRC-16:** 256-entry lookup table for sub-microsecond packet verification.
- **Zero Dynamic Allocation:** Operates strictly on pre-allocated static buffers.
- **Independent Hardware Layer:** Integrates directly with hardware timer (3.5 character timeout) and UART ISR.

---

## Running Unit Tests

```bash
make
```

---

## License
Released under the [MIT License](LICENSE). Designed by **Sherifred Singh**.
