#include "interrupts.h"

void _on_hard_fault(void) {
    uart_write_buf(USART2, "*HF*", 4);
    spin(50000);
}

void _on_mem_fault(void) {
    uart_write_buf(USART2, "*MF*", 4);
    spin(50000);
}

void _on_bus_fault(void) {
    uart_write_buf(USART2, "*BF*", 4);
    spin(50000);
}

void _on_usage_fault(void) {
    uart_write_buf(USART2, "*UF*", 4);
    spin(50000);
}

void _on_systick(void) {}

void _on_uart2_interrupt(void) {
    uint8_t byte = uart_read_byte(USART2);
    uart_write_byte(USART2, byte);
}