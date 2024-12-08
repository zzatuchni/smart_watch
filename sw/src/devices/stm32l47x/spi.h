#ifndef SPI_H
#define SPI_H

#include "basic.h"
#include "gpio.h"

#define SPI1_REGS_START_ADDRESS 0x40013000
#define SPI1_APB2ENR_BIT 12
#define SPI1_AF_NUMBER 5

#define SPI2_REGS_START_ADDRESS 0x40003800
#define SPI2_APB1ENR1_BIT 14
#define SPI2_AF_NUMBER 5

#define SPI3_REGS_START_ADDRESS 0x40003C00
#define SPI3_APB1ENR1_BIT 15
#define SPI3_AF_NUMBER 6

#define SPI1_BAUD_CONTROL 0

typedef struct {
    volatile uint32_t CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR;
} SPI_Regs;

#define SPI1 ((SPI_Regs *)SPI1_REGS_START_ADDRESS)
#define SPI2 ((SPI_Regs *)SPI2_REGS_START_ADDRESS)
#define SPI3 ((SPI_Regs *)SPI3_REGS_START_ADDRESS)

typedef struct {
    SPI_Regs *spi;
    STM32_Pin cs_pin;
    STM32_Pin sclk_pin;
    STM32_Pin miso_pin;
    STM32_Pin mosi_pin;
    STM32_Pin rst_pin;
    STM32_Pin dc_pin;
    bool rx_dma_enable;
    bool tx_dma_enable;
    bool lsb_first;
} SPI_Config;

Result spi_init(const SPI_Config *config);

void spi_write_byte(SPI_Regs *spi, uint8_t dat);

void spi_write_word(SPI_Regs *spi, uint16_t dat);

void spi_write_buf(SPI_Regs *spi, char *buf, size_t len);

void spi_wait_until_not_busy(SPI_Regs *spi); 

#endif