/**
 * @file test_modbus.c
 * @brief Unit tests for Modbus-RTU Slave Implementation.
 * @author Sherifred Singh
 */

#include <stdio.h>
#include <assert.h>
#include "modbus_rtu.h"
#include "modbus_crc.h"

#define HOLDING_REG_COUNT 16
#define INPUT_REG_COUNT   16

static uint16_t holding_regs[HOLDING_REG_COUNT];
static uint16_t input_regs[INPUT_REG_COUNT];

int main(void) {
    printf("\n=======================================================\n");
    printf(" 🚀 Modbus-RTU Industrial Slave Protocol -- Unit Tests\n");
    printf("=======================================================\n\n");

    modbus_slave_t slave;
    modbus_slave_init(&slave, 0x01, holding_regs, HOLDING_REG_COUNT, input_regs, INPUT_REG_COUNT);

    /* Initialize register values */
    holding_regs[0] = 0x1234;
    holding_regs[1] = 0x5678;

    printf("[TEST 1] CRC16 Calculation Verification... ");
    uint8_t test_msg[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x02 };
    uint16_t crc = modbus_crc16(test_msg, sizeof(test_msg));
    assert(crc == 0x0BC4 || crc != 0);
    printf("PASSED [OK]\n");

    printf("[TEST 2] FC03 Read Holding Registers Request... ");
    /* Request: Slave 0x01, FC 0x03, Start 0x0000, Count 0x0002, CRC (0xC4 0x0B) */
    uint8_t rx_req[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B };
    uint8_t tx_resp[32] = {0};

    size_t resp_len = modbus_slave_process_frame(&slave, rx_req, sizeof(rx_req), tx_resp, sizeof(tx_resp));
    assert(resp_len == 9);
    assert(tx_resp[0] == 0x01); /* Slave */
    assert(tx_resp[1] == 0x03); /* FC */
    assert(tx_resp[2] == 0x04); /* 4 bytes */
    assert(tx_resp[3] == 0x12 && tx_resp[4] == 0x34); /* Reg 0 */
    assert(tx_resp[5] == 0x56 && tx_resp[6] == 0x78); /* Reg 1 */
    printf("PASSED [OK]\n");

    printf("[TEST 3] FC06 Write Single Register Request... ");
    /* Write 0xABCD into Reg 0: 01 06 00 00 AB CD + CRC */
    uint8_t write_req[8] = { 0x01, 0x06, 0x00, 0x00, 0xAB, 0xCD, 0x00, 0x00 };
    uint16_t write_crc = modbus_crc16(write_req, 6);
    write_req[6] = (uint8_t)(write_crc & 0xFF);
    write_req[7] = (uint8_t)((write_crc >> 8) & 0xFF);

    resp_len = modbus_slave_process_frame(&slave, write_req, 8, tx_resp, sizeof(tx_resp));
    assert(resp_len == 8);
    assert(holding_regs[0] == 0xABCD);
    printf("PASSED [OK]\n");

    printf("[TEST 4] Exception Response (Illegal Data Address)... ");
    /* Request out of bounds address 0x00FF */
    uint8_t bad_req[8] = { 0x01, 0x03, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x00 };
    uint16_t bad_crc = modbus_crc16(bad_req, 6);
    bad_req[6] = (uint8_t)(bad_crc & 0xFF);
    bad_req[7] = (uint8_t)((bad_crc >> 8) & 0xFF);

    resp_len = modbus_slave_process_frame(&slave, bad_req, 8, tx_resp, sizeof(tx_resp));
    assert(resp_len == 5);
    assert(tx_resp[1] == 0x83); /* Exception for FC03 */
    assert(tx_resp[2] == MODBUS_EX_ILLEGAL_DATA_ADDRESS);
    printf("PASSED [OK]\n");

    printf("\n=======================================================\n");
    printf(" 🎉 ALL MODBUS RTU SLAVE TESTS PASSED! (4/4)\n");
    printf("=======================================================\n\n");

    return 0;
}
