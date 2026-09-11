/**
 * @file modbus_rtu.h
 * @brief Lightweight Modbus-RTU Slave Protocol Stack in C.
 * @author Sherifred Singh
 */

#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MODBUS_FC_READ_HOLDING_REGISTERS     0x03
#define MODBUS_FC_READ_INPUT_REGISTERS       0x04
#define MODBUS_FC_WRITE_SINGLE_REGISTER      0x06
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS   0x10

#define MODBUS_EX_ILLEGAL_FUNCTION           0x01
#define MODBUS_EX_ILLEGAL_DATA_ADDRESS       0x02
#define MODBUS_EX_ILLEGAL_DATA_VALUE         0x03

typedef struct {
    uint8_t slave_address;
    uint16_t *holding_registers;
    size_t holding_register_count;
    uint16_t *input_registers;
    size_t input_register_count;
} modbus_slave_t;

void modbus_slave_init(modbus_slave_t *slave,
                       uint8_t address,
                       uint16_t *holding_regs, size_t num_holding,
                       uint16_t *input_regs, size_t num_inputs);

size_t modbus_slave_process_frame(modbus_slave_t *slave,
                                  const uint8_t *rx_frame, size_t rx_len,
                                  uint8_t *tx_frame, size_t tx_max_len);

#endif /* MODBUS_RTU_H */
/* Industrial Modbus-RTU Slave Protocol Stack - Deterministic Frame Processing */
