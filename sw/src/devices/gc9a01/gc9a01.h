#ifndef GC9A01_H
#define GC9A01_H

#include "../stm32l47x/basic.h"
#include "../stm32l47x/uart.h"
#include "../stm32l47x/gpio.h"
#include "../stm32l47x/spi.h"

#include "gc9a01_constants.h"

extern const SPI_Config gc9a01_spi_config;

extern const uint8_t gc9a01_init_cmds[];

typedef uint16_t GC9A01_Color;

void gc9a01_write_cmd_code(uint8_t cmd);

void gc9a01_write_byte(uint8_t byte);

void gc9a01_write_color(GC9A01_Color color);

void gc9a01_write_buf(char *buf, size_t len);

void gc9a01_write_colors(GC9A01_Color *colors, size_t num_colors);

void gc9a01_write_cmd(uint8_t cmd, uint8_t *args, size_t num_args);

Result gc9a01_init();

Result gc9a01_set_frame(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

#endif