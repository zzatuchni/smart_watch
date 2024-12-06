#ifndef UART_H
#define UART_H

#include "basic.h"
#include "gpio.h"

#define UART_BAUD_RATE 115200

#define USART1_REGS_START_ADDRESS 0x40013800
#define USART1_APB2ENR_BIT 14
#define USART1_INTERRUPT_NUM 37

#define USART2_REGS_START_ADDRESS 0x40004400
#define USART2_APB1ENR1_BIT 17
#define USART2_INTERRUPT_NUM 38

#define USART3_REGS_START_ADDRESS 0x40004800
#define USART3_APB1ENR1_BIT 18
#define USART3_INTERRUPT_NUM 39

#define UART4_REGS_START_ADDRESS 0x40004C00
#define UART4_APB1ENR1_BIT 19
#define UART4_INTERRUPT_NUM 52

#define UART5_REGS_START_ADDRESS 0x40005000
#define UART5_APB1ENR1_BIT 20
#define UART5_INTERRUPT_NUM 33

#define LPUART1_REGS_START_ADDRESS 0x40008000
#define LPUART1_APB1ENR2_BIT 0
#define LPUART1_INTERRUPT_NUM 70

#define USART1_3_AF_NUMBER 7
#define UART4_5_AF_NUMBER 8

typedef struct {
    volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR, TDR;
} UART_Regs;

#define USART1 ((UART_Regs *)USART1_REGS_START_ADDRESS)
#define USART2 ((UART_Regs *)USART2_REGS_START_ADDRESS)
#define USART3 ((UART_Regs *)USART3_REGS_START_ADDRESS)
#define UART4 ((UART_Regs *)UART4_REGS_START_ADDRESS)
#define UART5 ((UART_Regs *)UART5_REGS_START_ADDRESS)

typedef struct {
    volatile uint32_t CR1, CR2, CR3, BRR, RQR, ISR, ICR, RDR, TDR;
} LPUART_Regs;

#define LPUART1 ((LPUART_Regs *)LPUART1_REGS_START_ADDRESS)

Result uart_init(UART_Regs *uart, STM32_Pin tx_pin, STM32_Pin rx_pin);

Result lpuart_init(STM32_Pin tx_pin, STM32_Pin rx_pin);

uint8_t uart_read_byte(UART_Regs *uart);

int uart_read_ready(UART_Regs *uart);

void uart_write_byte(UART_Regs *uart, uint8_t byte);

void uart_write_buf(UART_Regs *uart, char *buf, size_t len);

void write_value(UART_Regs *uart, uint32_t val);

#endif