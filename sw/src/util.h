#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define BIT(x) (1UL << (x))
#define BYTE_SWAP(x) ((((x) & 0xFF00) >> 8) + (((x) & 0x00FF) << 8))

typedef int Result;

 __attribute__((optimize("O0"))) void spin(uint32_t count);

void int_to_str(uint32_t x, char *buf, uint8_t size, uint8_t radix);

bool str_cmp(char *buf1, char *buf2, uint8_t size);

void str_copy(char *buf1, char *buf2, uint8_t size);

#endif