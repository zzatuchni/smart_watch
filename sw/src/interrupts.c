#include "interrupts.h"

void _on_hard_fault(void) {
    DPRINT("*HF*");
    spin(50000);
}

void _on_mem_fault(void) {
    DPRINT("*MF*");
    spin(50000);
}

void _on_bus_fault(void) {
    DPRINT("*BF*");
    spin(50000);
}

void _on_usage_fault(void) {
    DPRINT("*UF*");
    spin(50000);
}

void _on_systick(void) {}

void _on_uart2_interrupt(void) {}



void _on_button1_press(void) {
    EXTI_Regs *exti = (EXTI_Regs *)EXTI_REGS1_START_ADDRESS;
    exti->PR |= (1UL << BUTTON1_PIN_NO);

    button1_pressed = true;
}

//void _on_uart2_interrupt(void) {
//    uint8_t byte = uart_read_byte(USART2);
//    uart_write_byte(USART2, byte);
//}