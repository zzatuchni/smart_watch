#include "interrupts.h"
#include "devices/stm32l47x/basic.h"
#include "devices/stm32l47x/gpio.h"
#include "devices/stm32l47x/uart.h"
#include "devices/stm32l47x/rtc.h"
#include "devices/gc9a01/gc9a01.h"

const GC9A01_Color colors[] = {
    0x0000, 0x00FF, 0x0000, 0x00FF, 0x0000,
    0x00FF, 0x0000, 0x00FF, 0x0000, 0x00FF,
    0x0000, 0x00FF, 0x0000, 0x00FF, 0x0000,
    0x00FF, 0x0000, 0x00FF, 0x0000, 0x00FF,
    0x0000, 0x00FF, 0x0000, 0x00FF, 0x0000,
};

const uint8_t colors_mask[] = {
    0b01010101, 0b10101010,
    0b01010101, 0b10101010,
    0b01010101, 0b10101010,
    0b01010101, 0b10101010,
};

////////////////////////////////////////////////////////////
//  Startup code 
////////////////////////////////////////////////////////////

#define LED_PIN_BANK 'A'
#define LED_PIN_NUMBER 5

void led_init(STM32_Pin led_pin) { 
    gpio_set_mode(GPIO_MODE_OUTPUT, led_pin);
}

char buf[8];
char date_buf[64];


int main(void) {

    //systick_init(DEFAULT_SYSCLK_FREQ / 1000);

    //STM32_Pin led_pin = {LED_PIN_BANK, LED_PIN_NUMBER};
    //led_init(led_pin);

    const UART_Config uart_cfg = {
        USART2,     // uarts
        {'A', 2},   // tx_pin
        {'A', 3}    // rx_pin
    };

    Result res = uart_init(&uart_cfg);
    if (res) { for (;;) {} };
    uart_write_buf(USART2, "\33c", 2);

    res = gc9a01_init();
    if (res) { for (;;) {} };

    GC9A01_Frame frame = {0, 0, GC9A01A_TFTWIDTH-1, GC9A01A_TFTHEIGHT-1};
    
    rtc_init();

    Time time;    

    for (;;) {
        //res = gc9a01_draw_colors(colors, 25, &frame, 48);
        res = gc9a01_draw_colors_from_bitmask(colors_mask, 8, GC9A01A_MAGENTA, GC9A01A_BLACK, &frame, 30);
        if (res == RES_BAD_PARAM) uart_write_buf(USART2, "LCDBP!\r\n", 8);
        if (res == RES_OUT_OF_BOUNDS) uart_write_buf(USART2, "LCDOB!\r\n", 8);

        rtc_get_time(&time);
        
        int_to_str(time.weekday, buf, 8, 10);
        uart_write_buf(USART2, "WD ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.year, buf, 8, 10);
        uart_write_buf(USART2, " Y ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.month, buf, 8, 10);
        uart_write_buf(USART2, " M ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.day, buf, 8, 10);
        uart_write_buf(USART2, " D ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.hour, buf, 8, 10);
        uart_write_buf(USART2, " h ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.minute, buf, 8, 10);
        uart_write_buf(USART2, " m ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.second, buf, 8, 10);
        uart_write_buf(USART2, " s ", 3);
        uart_write_buf(USART2, buf, 8);

        int_to_str(time.subsecond, buf, 8, 10);
        uart_write_buf(USART2, " ss ", 4);
        uart_write_buf(USART2, buf, 8);

        uart_write_buf(USART2, "\r\n", 2);

        spin(1000000);
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