#include "interrupts.h"
#include "devices/stm32l47x/basic.h"
#include "devices/stm32l47x/gpio.h"
#include "devices/stm32l47x/uart.h"
#include "devices/gc9a01/gc9a01.h"

////////////////////////////////////////////////////////////
//  Startup code 
////////////////////////////////////////////////////////////

#define LED_PIN_BANK 'A'
#define LED_PIN_NUMBER 5

void led_init(STM32_Pin led_pin) { 
    gpio_set_mode(GPIO_MODE_OUTPUT, led_pin);
}

int main(void) {

    //systick_init(DEFAULT_SYSCLK_FREQ / 1000);

    //STM32_Pin led_pin = {LED_PIN_BANK, LED_PIN_NUMBER};
    //led_init(led_pin);

    const UART_Config uart_cfg = {
        USART2,     // uart
        {'A', 2},   // tx_pin
        {'A', 3}    // rx_pin
    };

    Result res = uart_init(&uart_cfg);
    if (res) { for (;;) {} };

    res = gc9a01_init();
    if (res) { for (;;) { 
        uart_write_value(USART2, res);
        uart_write_buf(USART2, " bad lcd\r\n", 10); 
        spin(50000);
    } };

    uint16_t color = GC9A01A_BLUE;

    gc9a01_set_frame(0, 0, 240, 240);
    gc9a01_write_cmd_code(GC9A01A_RAMWR);

    for (;;) {
        gc9a01_write_colors(&color, 1);
    }

    return 0;
}

__attribute__((naked, noreturn)) void _reset(void) {
  // memset .bss to zero, and copy .data section to RAM region
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

  main();             // Call main()
  for (;;) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);  // Defined in link.ld

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const volatile tab[16 + 99])(void) = {
    _estack, _reset, 0, _on_hard_fault, _on_mem_fault, _on_bus_fault, _on_usage_fault, 0, 0, 0, 0, 0, 0, 0, 0, _on_systick,  //ARM core interrupts
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0 - 15
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16 - 31
    0, 0, 0, 0, 0, 0, _on_uart2_interrupt, 0, 0x0, 0, 0, 0, 0, 0, 0, 0, // 32 - 47
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 48 - 63
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64 - 79
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80 - 96
    };