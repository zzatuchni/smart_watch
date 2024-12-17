#ifndef UART_H
#define UART_H

#include "basic.h"
#include "gpio.h"

extern char temp_number_buffer[32];

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

typedef struct {
    UART_Regs *uart;
    STM32_Pin tx_pin;
    STM32_Pin rx_pin;
} UART_Config;

typedef struct {
    STM32_Pin tx_pin;
    STM32_Pin rx_pin;
} LPUART_Config;

Result uart_init(const UART_Config *config);

Result lpuart_init(const LPUART_Config *config);

uint8_t uart_read_byte(UART_Regs *uart);

int uart_read_ready(UART_Regs *uart);

void uart_write_byte(UART_Regs *uart, uint8_t byte);

void uart_write_buf(UART_Regs *uart, char *buf, size_t len);

void uart_write_number(UART_Regs *uart, uint32_t val, uint8_t radix);

#define DPRINT(x) uart_write_buf(USART2, (x), sizeof((x)));
#define DPRINTLN(x) { uart_write_buf(USART2, (x), sizeof((x))); uart_write_buf(USART2, "\r\n", 2); }
#define DPRINTN(x) uart_write_number(USART2, (x), 10);
#define DPRINTBUF(x,sz) uart_write_buf(USART2, (x), (sz));
#define DPRINTB(x) uart_write_number(USART2, (x), 2);
#define DPRINTH(x) uart_write_number(USART2, (x), 8);
#define DPRINTNL() uart_write_buf(USART2, "\r\n", 2);
#define DCLRSCRN() uart_write_buf(USART2, "\33c", 2);

#define ENABLE_DBG_TRACES() {\
    const UART_Config uart_cfg = {\
        USART2,\
        {'A', 2},\
        {'A', 3}\
    };\
    Result res = uart_init(&uart_cfg);\
    if (res) { for (;;) {} };\
    DCLRSCRN();\
    DPRINTLN("DEBUG TRACES ON");\
}

#endif