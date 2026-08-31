/**
 * @file modbus_crc.h
 * @brief High-speed Table-Driven Modbus RTU CRC-16 Calculation.
 * @author Sherifred Singh
 */

#ifndef MODBUS_CRC_H
#define MODBUS_CRC_H

#include <stdint.h>
#include <stddef.h>

uint16_t modbus_crc16(const uint8_t *buffer, size_t length);

#endif /* MODBUS_CRC_H */
