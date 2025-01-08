#ifndef GC9A01_H
#define GC9A01_H

#include "../stm32l47x/basic.h"
#include "../stm32l47x/uart.h"
#include "../stm32l47x/gpio.h"
#include "../stm32l47x/spi.h"

#include "gc9a01_constants.h"

typedef uint16_t GC9A01_Color;

typedef struct { uint8_t x; uint8_t y; uint8_t sx; uint8_t sy; } GC9A01_Frame;

Result gc9a01_init();

Result gc9a01_set_frame(GC9A01_Frame *frame);

Result gc9a01_draw_colors(GC9A01_Color const *colors, size_t num_colors, GC9A01_Frame *frame, uint8_t scale);

Result gc9a01_draw_colors_from_bitmask(uint8_t const *mask, size_t len_bytes, 
    GC9A01_Color fg_color, GC9A01_Color bg_color, GC9A01_Frame *frame, uint8_t scale);

#endif