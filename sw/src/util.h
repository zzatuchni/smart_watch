#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define BIT(x) (1UL << (x))
#define BYTE_SWAP(x) ((((x) & 0xFF00) >> 8) + (((x) & 0x00FF) << 8))

#define GENERIC_TIMEOUT_NUM 2000000
typedef enum {
    RES_OK, RES_TIMEOUT, RES_BAD_PARAM, RES_NOT_SUPPORTED
} Result;

 __attribute__((optimize("O0"))) void spin(uint32_t count);

void int_to_str(uint32_t x, char *buf, size_t size, uint8_t radix);

bool str_cmp(char *buf1, char *buf2, size_t size);

void str_copy(char *buf1, char *buf2, size_t size);

#endif