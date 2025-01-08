#ifndef I2C_H
#define I2C_H

#include "basic.h"
#include "gpio.h"

#define I2C1_REGS_START_ADDRESS 0x40005400
#define I2C2_REGS_START_ADDRESS 0x40005800
#define I2C3_REGS_START_ADDRESS 0x40005C00

#define I2C1_APB1ENR1_BIT 21
#define I2C2_APB1ENR1_BIT 22
#define I2C3_APB1ENR1_BIT 23

#define I2C_AF_NUMBER 4

typedef uint8_t I2C_Address;

typedef struct {
    volatile uint32_t CR1, CR2, OAR1, OAR2, TIMINGR, TIMOUTGR, 
        ISR, ICR, PECR, RXDR, TXDR;
} I2C_Regs;

#define I2C1 ((I2C_Regs *)I2C1_REGS_START_ADDRESS)
#define I2C2 ((I2C_Regs *)I2C2_REGS_START_ADDRESS)
#define I2C3 ((I2C_Regs *)I2C3_REGS_START_ADDRESS)

// Standard mode, 4khz clock
#define I2C_TIMING 0x004091F3

typedef struct {
    I2C_Regs *i2c;
    STM32_Pin scl_pin;
    STM32_Pin sda_pin;
} I2C_Config;

// Only doing master mode right now
Result i2c_init(const I2C_Config *config) {

    uint8_t af_num = I2C_AF_NUMBER;

    switch ((uint32_t)config->i2c) {
        case I2C1_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(I2C1_APB1ENR1_BIT);
            break;
        case I2C2_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(I2C2_APB1ENR1_BIT);
            break;
        case I2C3_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(I2C3_APB1ENR1_BIT);
            break;
        default:
            return RES_BAD_PARAM;
    }

    gpio_set_mode(GPIO_MODE_AF, config->scl_pin);
    gpio_set_speed(GPIO_SPEED_HIGH, config->scl_pin);
    gpio_set_pupdr(GPIO_PP_PULL_UP, config->scl_pin);
    gpio_set_af(af_num, config->scl_pin);
    gpio_set_otyper(true, config->scl_pin);

    gpio_set_mode(GPIO_MODE_AF, config->sda_pin);
    gpio_set_speed(GPIO_SPEED_HIGH, config->sda_pin);
    gpio_set_pupdr(GPIO_PP_PULL_UP, config->sda_pin);
    gpio_set_af(af_num, config->sda_pin);
    gpio_set_otyper(true, config->sda_pin);

    config->i2c->CR1 &= ~BIT(0); // PE - peripheral enable
    WAIT_FOR_CONDITION(~(config->i2c->CR1 & BIT(0)), GENERIC_TIMEOUT_NUM);

    config->i2c->TIMOUTGR = I2C_TIMING; // set timing

    config->i2c->CR1 |= BIT(0); // PE - peripheral enable
}

Result i2c_write_bytes(I2C_Regs *i2c, I2C_Address addr, char *buf, size_t len) {

    // set slave address

    // set transfer direction to write

    // set nbytes to transfer

    // enable START bit

    // begin transfer
    // while true / until timeout
        // if nackf received, return error

        // if txis not set, continue

        // write byte to txdr

        // if nbytes not transmitted yet, continue

        // if tc bit (transfer complete) set, break else return err

}

void i2c_read_bytes(I2C_Regs *i2c, I2C_Address addr, char *buf, size_t len) {

    // set slave address

    // set transfer direction to write

    // set nbytes to transfer

    // enable START bit

    // begin transfer
    // while true / until timeout
        // if rxne not set, continue

        // read byte from rxdr

        // if nbytes not received yet, continue

        // if tc bit (transfer complete) set, break else return error

}

#endif