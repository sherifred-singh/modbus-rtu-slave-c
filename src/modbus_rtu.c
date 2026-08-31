/**
 * @file modbus_rtu.c
 * @brief Implementation of Modbus-RTU Slave Protocol.
 * @author Sherifred Singh
 */

#include "modbus_rtu.h"
#include "modbus_crc.h"

void modbus_slave_init(modbus_slave_t *slave,
                       uint8_t address,
                       uint16_t *holding_regs, size_t num_holding,
                       uint16_t *input_regs, size_t num_inputs) {
    if (!slave) return;
    slave->slave_address = address;
    slave->holding_registers = holding_regs;
    slave->holding_register_count = num_holding;
    slave->input_registers = input_regs;
    slave->input_register_count = num_inputs;
}

static size_t build_exception(uint8_t slave_addr, uint8_t function_code, uint8_t exception_code, uint8_t *tx_frame) {
    tx_frame[0] = slave_addr;
    tx_frame[1] = function_code | 0x80;
    tx_frame[2] = exception_code;
    uint16_t crc = modbus_crc16(tx_frame, 3);
    tx_frame[3] = (uint8_t)(crc & 0xFF);        /* CRC Lo */
    tx_frame[4] = (uint8_t)((crc >> 8) & 0xFF); /* CRC Hi */
    return 5;
}

size_t modbus_slave_process_frame(modbus_slave_t *slave,
                                  const uint8_t *rx_frame, size_t rx_len,
                                  uint8_t *tx_frame, size_t tx_max_len) {
    if (!slave || !rx_frame || !tx_frame || rx_len < 4 || tx_max_len < 5) {
        return 0;
    }

    /* Check Slave Address (Ignore frames for other nodes or broadcasts for now) */
    if (rx_frame[0] != slave->slave_address) {
        return 0;
    }

    /* Validate CRC16 */
    uint16_t calculated_crc = modbus_crc16(rx_frame, rx_len - 2);
    uint16_t received_crc = (uint16_t)(rx_frame[rx_len - 2] | (rx_frame[rx_len - 1] << 8));
    if (calculated_crc != received_crc) {
        return 0; /* Drop frame with invalid CRC silently */
    }

    uint8_t function_code = rx_frame[1];

    /* Function 03: Read Holding Registers */
    if (function_code == MODBUS_FC_READ_HOLDING_REGISTERS) {
        uint16_t start_addr = (uint16_t)((rx_frame[2] << 8) | rx_frame[3]);
        uint16_t reg_count  = (uint16_t)((rx_frame[4] << 8) | rx_frame[5]);

        if (reg_count < 1 || reg_count > 125) {
            return build_exception(slave->slave_address, function_code, MODBUS_EX_ILLEGAL_DATA_VALUE, tx_frame);
        }
        if ((start_addr + reg_count) > slave->holding_register_count) {
            return build_exception(slave->slave_address, function_code, MODBUS_EX_ILLEGAL_DATA_ADDRESS, tx_frame);
        }

        uint8_t byte_count = (uint8_t)(reg_count * 2);
        tx_frame[0] = slave->slave_address;
        tx_frame[1] = function_code;
        tx_frame[2] = byte_count;

        size_t idx = 3;
        for (uint16_t i = 0; i < reg_count; i++) {
            uint16_t val = slave->holding_registers[start_addr + i];
            tx_frame[idx++] = (uint8_t)((val >> 8) & 0xFF);
            tx_frame[idx++] = (uint8_t)(val & 0xFF);
        }

        uint16_t crc = modbus_crc16(tx_frame, idx);
        tx_frame[idx++] = (uint8_t)(crc & 0xFF);
        tx_frame[idx++] = (uint8_t)((crc >> 8) & 0xFF);
        return idx;
    }

    /* Function 06: Write Single Register */
    if (function_code == MODBUS_FC_WRITE_SINGLE_REGISTER) {
        uint16_t reg_addr = (uint16_t)((rx_frame[2] << 8) | rx_frame[3]);
        uint16_t reg_val  = (uint16_t)((rx_frame[4] << 8) | rx_frame[5]);

        if (reg_addr >= slave->holding_register_count) {
            return build_exception(slave->slave_address, function_code, MODBUS_EX_ILLEGAL_DATA_ADDRESS, tx_frame);
        }

        slave->holding_registers[reg_addr] = reg_val;

        /* Echo request as response */
        for (size_t i = 0; i < 6; i++) {
            tx_frame[i] = rx_frame[i];
        }
        uint16_t crc = modbus_crc16(tx_frame, 6);
        tx_frame[6] = (uint8_t)(crc & 0xFF);
        tx_frame[7] = (uint8_t)((crc >> 8) & 0xFF);
        return 8;
    }

    /* Unsupported Function */
    return build_exception(slave->slave_address, function_code, MODBUS_EX_ILLEGAL_FUNCTION, tx_frame);
}
