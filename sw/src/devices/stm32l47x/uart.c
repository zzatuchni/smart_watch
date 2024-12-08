#include "uart.h"

Result uart_init(const UART_Config *config) {

    // enable peripheral in RCC and get af_num and interrupt_num

    uint8_t af_num = 0;
    uint8_t interrupt_num = 0;

    switch ((uint32_t)config->uart) {
        case USART1_REGS_START_ADDRESS:
            RCC->APB2ENR |= BIT(USART1_APB2ENR_BIT);
            af_num = USART1_3_AF_NUMBER;
            interrupt_num = USART1_INTERRUPT_NUM;
            break;
        case USART2_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(USART2_APB1ENR1_BIT);
            af_num = USART1_3_AF_NUMBER;
            interrupt_num = USART2_INTERRUPT_NUM;
            break;
        case USART3_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(USART3_APB1ENR1_BIT);
            af_num = USART1_3_AF_NUMBER;
            interrupt_num = USART3_INTERRUPT_NUM;
            break;
        case UART4_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(UART4_APB1ENR1_BIT);
            af_num = UART4_5_AF_NUMBER;
            interrupt_num = UART4_INTERRUPT_NUM;
            break;
        case UART5_REGS_START_ADDRESS:
            RCC->APB1ENR1 |= BIT(UART5_APB1ENR1_BIT);
            af_num = UART4_5_AF_NUMBER;
            interrupt_num = UART5_INTERRUPT_NUM;
            break;
        default:
            return RES_BAD_PARAM;
    }

    // configure the pin gpio

    gpio_set_mode(GPIO_MODE_AF, config->tx_pin);
    gpio_set_mode(GPIO_MODE_AF, config->rx_pin);

    gpio_set_af(af_num, config->tx_pin);
    gpio_set_af(af_num, config->rx_pin);

    // configure the uart regs

    config->uart->CR1 = 0;                                      // Disable this UART
    config->uart->BRR = DEFAULT_SYSCLK_FREQ / UART_BAUD_RATE;   // DEFAULT_SYSCLK_FREQ is the UART bus frequency
    config->uart->CR1 |= BIT(0) | BIT(2) | BIT(3) | BIT(5);     // Set UE, RE, TE, RXNEIE (uart enable, rx enable, tx enable, rx interrupt enable)

    // enable interrupt on receiving byte

    enable_interrupt(interrupt_num);

    return RES_OK;
}

Result lpuart_init(const LPUART_Config *config) {

    // enable peripheral in RCC
    RCC->APB1ENR2 |= BIT(LPUART1_APB1ENR2_BIT);
    uint8_t af_num = UART4_5_AF_NUMBER;
    uint8_t interrupt_num = LPUART1_INTERRUPT_NUM;

    gpio_set_mode(GPIO_MODE_AF, config->tx_pin);
    gpio_set_mode(GPIO_MODE_AF, config->rx_pin);

    gpio_set_af(af_num, config->tx_pin);
    gpio_set_af(af_num, config->rx_pin);

    LPUART_Regs *uart = LPUART1;

    uart->CR1 = 0;                                      // Disable this UART
    uart->BRR = DEFAULT_SYSCLK_FREQ / UART_BAUD_RATE;   // FREQ is a UART bus frequency
    uart->CR1 |= BIT(0) | BIT(2) | BIT(3) | BIT(5);     // Set UE, RE, TE, RXNEIE

    enable_interrupt(interrupt_num);

    return RES_OK;
}

uint8_t uart_read_byte(UART_Regs *uart) {
    return (uint8_t) (uart->RDR & 255);
}

int uart_read_ready(UART_Regs *uart) {
    return (uart->ISR & BIT(5)); // RXNE
}

void uart_write_byte(UART_Regs *uart, uint8_t byte) {
    uart->TDR = byte;
    while ((uart->ISR & BIT(6)) == 0) spin(1); // TC - transmission complete
}

void uart_write_buf(UART_Regs *uart, char *buf, size_t len) {
    while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}

void uart_write_value(UART_Regs *uart, uint32_t val) {
    for (uint8_t i = 0; i < 32; i++) {
        uint32_t b = val & BIT(0);
        if (b) uart_write_byte(uart, '1'); else uart_write_byte(uart, '0');
        val = val >> 0x1U;
    }
    uart_write_byte(uart, '\r');
    uart_write_byte(uart, '\n');
}