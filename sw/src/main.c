#include "devices/stm32l47x/basic.h"
#include "devices/stm32l47x/gpio.h"
#include "devices/stm32l47x/uart.h"
#include "devices/stm32l47x/rtc.h"

#include "devices/gc9a01/gc9a01.h"
#include "devices/sht41/sht41.h"

#include "images/test_font.h"

#include "interrupts.h"
#include "debug.h"

////////////////////////////////////////////////////////////
//  Startup code 
////////////////////////////////////////////////////////////

#define LED_PIN_BANK 'A'
#define LED_PIN_NUMBER 5

void led_init(STM32_Pin led_pin) { 
    gpio_set_mode(GPIO_MODE_OUTPUT, led_pin);
}

char date_buf[6] = {'1','2','3','4','5','6'};
char date_buf_prev[6] = {'1','2','3','4','5','6'};
char temp_hum_buf[6] = {0};

const uint8_t *character_bitmap_bufs[10] = 
    {character_0, character_1, character_2, character_3, character_4, character_5, character_6, character_7, character_8, character_9};

GC9A01_Frame bg_frame = {0, 0, GC9A01A_TFTWIDTH, GC9A01A_TFTHEIGHT};
GC9A01_Frame ht_frame = {168, 40, 48, 96};
GC9A01_Frame hu_frame = {120, 40, 48, 96};
GC9A01_Frame mt_frame = {72, 40, 48, 96};
GC9A01_Frame mu_frame = {24, 40, 48, 96};

GC9A01_Frame st_frame = {120, 136, 48, 96};
GC9A01_Frame su_frame = {72, 136, 48, 96};

void dbg_print_date(RTC_Time *time) {
    DPRINT("WD ");
    DPRINTN(time->weekday);

    DPRINT(" Y ");
    DPRINTN(time->year);

    DPRINT(" M ");
    DPRINTN(time->month);

    DPRINT(" D ");
    DPRINTN(time->day);

    DPRINT(" h ");
    DPRINTN(time->hour);

    DPRINT(" m ");
    DPRINTN(time->minute);

    DPRINT(" s ");
    DPRINTN(time->second);

    DPRINT(" ss ");
    DPRINTN(time->subsecond);

    DPRINTNL();
}

int main(void) {
    ENABLE_DBG_TRACES();

    systick_init(DEFAULT_SYSCLK_FREQ / 1000);

    //STM32_Pin led_pin = {LED_PIN_BANK, LED_PIN_NUMBER};
    //led_init(led_pin);

    Result res = gc9a01_init();
    if (res) { DPRINT("LCD INIT ERR "); DPRINTN(res); for (;;) {} };
    
    res = rtc_init();
    if (res) { DPRINT("RTC INIT ERR "); DPRINTN(res); for (;;) {} };

    res = sht41_init();
    if (res) { DPRINT("SHT41 INIT ERR "); DPRINTN(res); for (;;) {} };

    res = sht41_get_temp_hum_data(&temp_hum_buf, SHT41_HI_PREC);
    if (res) { DPRINT("GET TEMP ERR "); DPRINTN(res); for (;;) {} };

    for (size_t i = 0; i < 6; i++) {
        DPRINTH(temp_hum_buf[i]);
    }

    RTC_Time time;    

    // draw the backround
    res = gc9a01_draw_colors_from_bitmask(checkerboard, 29, GC9A01A_BLUE2, GC9A01A_BLUE3, &bg_frame, 16);

    for (;;) {
        rtc_get_time(&time);

        int_to_str(time.hour, date_buf, 2, 10);
        int_to_str(time.minute, date_buf + 2, 2, 10);
        int_to_str(time.second, date_buf + 4, 2, 10);

        if (!str_cmp(date_buf, date_buf_prev, 2)) {
            gc9a01_draw_colors_from_bitmask(character_bitmap_bufs[date_buf[0]-48], 16, GC9A01A_WHITE, GC9A01A_BLACK, &ht_frame, 6);
            gc9a01_draw_colors_from_bitmask(character_bitmap_bufs[date_buf[1]-48], 16, GC9A01A_RED, GC9A01A_BLACK, &hu_frame, 6);
        }

        if (!str_cmp(date_buf+2, date_buf_prev+2, 2)) {
            gc9a01_draw_colors_from_bitmask(character_bitmap_bufs[date_buf[2]-48], 16, GC9A01A_WHITE, GC9A01A_BLACK, &mt_frame, 6);
            gc9a01_draw_colors_from_bitmask(character_bitmap_bufs[date_buf[3]-48], 16, GC9A01A_RED, GC9A01A_BLACK, &mu_frame, 6);
        }

        if (!str_cmp(date_buf+4, date_buf_prev+4, 2)) {
            gc9a01_draw_colors_from_bitmask(character_bitmap_bufs[date_buf[4]-48], 16, GC9A01A_WHITE, GC9A01A_BLACK, &st_frame, 6);
            gc9a01_draw_colors_from_bitmask(character_bitmap_bufs[date_buf[5]-48], 16, GC9A01A_RED, GC9A01A_BLACK, &su_frame, 6);   
        }

        str_copy(date_buf, date_buf_prev, 6);

        DPRINTNL();

        spin(2000);
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