#include "i2c.h"

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

    return RES_OK;
}

Result i2c_write_buf(I2C_Regs *i2c, I2C_Address addr, char *buf, size_t len) {

    // set slave address
    i2c->CR2 |= addr;

    // set transfer direction to write
    i2c->CR2 &= ~BIT(10);

    // set nbytes to transfer
    i2c->CR2 |= (uint32_t)len << 16;

    // auto stop
    //i2c->CR2 &= ~BIT(25);

    // enable START bit
    i2c->CR2 |= BIT(13);

    // begin transfer
    while(len-- > 0) {
        DPRINTB(i2c->CR2);
        DPRINTNL();
        DPRINTB(i2c->ISR);
        DPRINTNL();

        // if nackf received, return error
        if (i2c->ISR & BIT(4)) {
            // set nackcf
            i2c->ICR |= BIT(4);
            return RES_WRITE_NACK;
        }

        // if txe not set, continue
        WAIT_FOR_CONDITION(i2c->ISR & BIT(0), GENERIC_TIMEOUT_NUM);

        // write byte to txdr
        i2c->TXDR = *(uint8_t *) buf++;
    }

    // check tc bit
    WAIT_FOR_CONDITION(i2c->ISR & BIT(6), GENERIC_TIMEOUT_NUM);
    // stop bit
    i2c->CR2 |= BIT(14);  
    return RES_OK;
}

Result i2c_read_buf(I2C_Regs *i2c, I2C_Address addr, char *buf, size_t len) {

    // set slave address
    i2c->CR2 |= addr;

    // set transfer direction to read
    i2c->CR2 |= BIT(10);

    // set nbytes to transfer
    i2c->CR2 |= (uint32_t)len << 16;

    // enable START bit
    i2c->CR2 |= BIT(13);

    // begin transfer
    for (size_t i = 0; i < len; i++) {
        // if nackf received, return error
        if (i2c->ISR & BIT(4)) {
            // set nackcf
            i2c->ICR |= BIT(4);
            return RES_READ_NACK;
        }

        // if rxne not set, continue
        WAIT_FOR_CONDITION(i2c->ISR & BIT(2), GENERIC_TIMEOUT_NUM);

        // read byte from rxdr
        buf[i] = i2c->RXDR;
    }

    // check tc bit
    WAIT_FOR_CONDITION(i2c->ISR & BIT(6), GENERIC_TIMEOUT_NUM);
    // stop bit
    i2c->CR2 |= BIT(14);  
    return RES_OK;
}