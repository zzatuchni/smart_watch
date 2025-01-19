#ifndef SHT41_H
#define SHT41_H

#include "../stm32l47x/basic.h"
#include "../stm32l47x/uart.h"
#include "../stm32l47x/gpio.h"
#include "../stm32l47x/i2c.h"

#define SHT41_I2C_ADDRESS 0x44
#define SHT41_GET_TEMP_HUM_HI_PREC_CMD 0xFD
#define SHT41_GET_TEMP_HUM_MED_PREC_CMD 0xF6
#define SHT41_GET_TEMP_HUM_LOW_PREC_CMD 0xE0

typedef enum { 
    SHT41_HI_PREC, SHT41_MED_PREC, SHT41_LOW_PREC 
} SHT41_Precision;

typedef struct {
    
} Temp_Humidity_Data;

Result sht41_get_temp_hum_data(char *buf, SHT41_Precision prec);

#endif