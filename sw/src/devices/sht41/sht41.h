#ifndef SHT41_H
#define SHT41_H

#include "../stm32l47x/basic.h"
#include "../stm32l47x/uart.h"
#include "../stm32l47x/gpio.h"
#include "../stm32l47x/i2c.h"

Result sht41_init();

Result sht41_get_temp_hum_data(char *buf);

#endif