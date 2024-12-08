#include "spi.h"

Result spi_init(const SPI_Config *config) {

    // enable peripheral in RCC and get af_num

    uint8_t af_num;

    switch ((uint32_t)config->spi) {
        case SPI1_REGS_START_ADDRESS:
            RCC->APB2ENR |= BIT(SPI1_APB2ENR_BIT);
            af_num = SPI1_AF_NUMBER;
            break;
        case SPI2_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(SPI2_APB1ENR1_BIT);
            af_num = SPI2_AF_NUMBER;
            break;
        case SPI3_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(SPI3_APB1ENR1_BIT);
            af_num = SPI3_AF_NUMBER;
            break;
        default:
            return -1;
    }

    // configure the pin gpio

    gpio_set_mode(GPIO_MODE_OUTPUT, config->cs_pin);
    gpio_set_pupdr(GPIO_PP_PULL_DOWN_ONLY, config->cs_pin);
    gpio_set_otyper(false, config->cs_pin);

    gpio_set_mode(GPIO_MODE_AF, config->sclk_pin);
    gpio_set_speed(GPIO_SPEED_HIGH, config->sclk_pin);
    gpio_set_pupdr(GPIO_PP_PULL_UP, config->sclk_pin);
    gpio_set_af(af_num, config->sclk_pin);
    gpio_set_otyper(false, config->sclk_pin);

    gpio_set_mode(GPIO_MODE_AF, config->miso_pin);
    gpio_set_speed(GPIO_SPEED_HIGH, config->miso_pin);
    gpio_set_pupdr(GPIO_PP_PULL_UP, config->miso_pin);
    gpio_set_af(af_num, config->miso_pin);
    gpio_set_otyper(false, config->miso_pin);

    gpio_set_mode(GPIO_MODE_AF, config->mosi_pin);
    gpio_set_speed(GPIO_SPEED_HIGH, config->mosi_pin);
    gpio_set_pupdr(GPIO_PP_PULL_UP, config->mosi_pin);
    gpio_set_af(af_num, config->mosi_pin);
    gpio_set_otyper(false, config->mosi_pin);

    gpio_set_mode(GPIO_MODE_OUTPUT, config->rst_pin);
    gpio_set_pupdr(GPIO_PP_PULL_DOWN_ONLY, config->rst_pin);
    gpio_set_otyper(false, config->rst_pin);

    gpio_set_mode(GPIO_MODE_OUTPUT, config->dc_pin);
    gpio_set_speed(GPIO_SPEED_HIGH, config->dc_pin);
    gpio_set_pupdr(GPIO_PP_PULL_DOWN_ONLY, config->dc_pin);
    gpio_set_otyper(false, config->dc_pin);
    
    // set pins to initial values

    gpio_write(true, config->cs_pin);
    gpio_write(true, config->dc_pin);
    gpio_write(true, config->sclk_pin);

    gpio_write(false, config->rst_pin);
    spin(500000);
    gpio_write(true, config->rst_pin);
    spin(500000);

    // configure SPI regs

    config->spi->CR1 &= ~BIT(6);   // SPE, disable SPI

    // reset peripheral at RCC
    switch ((uint32_t)config->spi) {
        case SPI1_REGS_START_ADDRESS:
            RCC->APB2RSTR |= BIT(SPI1_APB2ENR_BIT);
            RCC->APB2RSTR &= ~BIT(SPI1_APB2ENR_BIT);
            break;
        case SPI2_REGS_START_ADDRESS:
            RCC->APB1RSTR1 |= BIT(SPI2_APB1ENR1_BIT);
            RCC->APB1RSTR1 &= ~BIT(SPI2_APB1ENR1_BIT);
            break;
        case SPI3_REGS_START_ADDRESS:
            RCC->APB1RSTR1 |= BIT(SPI3_APB1ENR1_BIT);
            RCC->APB1RSTR1 &= ~BIT(SPI3_APB1ENR1_BIT);
            break;
        default:
            return -1;
    }

    // CPHA (second transition = first data capture), CPOL (CLK = 1 when idle), MSTR (master config)
    // SSI (internal slave select), SSM (software slave management enabled)
    config->spi->CR1 |= BIT(0) | BIT(1) | BIT(2) | BIT(8) | BIT(9);
    if (config->lsb_first)
        config->spi->CR1 |= BIT(7); // LSBFIRST
    

    config->spi->CR1 |= (SPI1_BAUD_CONTROL << 3);

    if (config->rx_dma_enable)
        config->spi->CR2 |= BIT(0); // enable DMA RX
    if (config->tx_dma_enable)
        config->spi->CR2 |= BIT(1); // enable DMA TX

    config->spi->CR1 |= BIT(6);    // SPE, enable SPI

    gpio_write(false, config->cs_pin);

    return 0;
}

void spi_write_byte(SPI_Regs *spi, uint8_t dat) {
    while (!(spi->SR & BIT(1))) {}; // TXE, transmit buffer empty
    *(uint8_t*)&(spi->DR) = dat;
}

void spi_write_word(SPI_Regs *spi, uint16_t dat) {
    while (!(spi->SR & BIT(1))) {}; // TXE, transmit buffer empty
    *(uint16_t*)&(spi->DR) = dat;
}

void spi_write_buf(SPI_Regs *spi, char *buf, size_t len) {
    while (len-- > 0) spi_write_byte(spi, *(uint8_t *) buf++);
}

void spi_wait_until_not_busy(SPI_Regs *spi) {
    while ((spi->SR & BIT(7))) {};
}