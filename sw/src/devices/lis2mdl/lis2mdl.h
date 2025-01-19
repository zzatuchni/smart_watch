#ifndef LIS2MDL_H
#define LIS2MDL_H

#include "../stm32l47x/basic.h"
#include "../stm32l47x/uart.h"
#include "../stm32l47x/gpio.h"
#include "../stm32l47x/i2c.h"

#define LIS2MDL_I2C_ADDRESS 0x1E

#define LIS2MDL_CFG_REG_A_ADDRESS 0x60
#define LIS2MDL_CFG_REG_B_ADDRESS 0x61
#define LIS2MDL_CFG_REG_C_ADDRESS 0x62

#define LIS2MDL_STATUS_REG_ADDRESS 0x67

#define LIS2MDL_OUTX_L_REG_ADDRESS 0x68
#define LIS2MDL_OUTX_H_REG_ADDRESS 0x69

#define LIS2MDL_OUTY_L_REG_ADDRESS 0x6A
#define LIS2MDL_OUTY_H_REG_ADDRESS 0x6B

#define LIS2MDL_OUTZ_L_REG_ADDRESS 0x6C
#define LIS2MDL_OUTZ_H_REG_ADDRESS 0x6D

#define LIS2MDL_CFG_REG_A_STARTUP_VALUE 0x80
#define LIS2MDL_CFG_REG_C_STARTUP_VALUE 0x01

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Magnetometer_Data;

Result lis2mdl_init();

Result lis2mdl_get_data(Magnetometer_Data *mgnmtr_data);

#endif