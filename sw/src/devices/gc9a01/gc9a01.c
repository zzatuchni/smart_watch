#include "gc9a01.h"

const SPI_Config gc9a01_spi_config = {
    SPI1,           // spi
    {'B', 6},       // cs_pin,          NUCLEO D10
    {'A', 5},       // sclk_pin         NUCLEO D13
    {'A', 6},       // miso_pin         NUCLEO D12
    {'A', 7},       // mosi_pin         NUCLEO D11
    {'A', 8},       // rst_pin          NUCLEO D7
    {'A', 10},      // dc_pin           NUCLEO D2
    false,          // rx_dma_enable
    false,          // tx_dma_enable
    false            // lsb first
};

const uint8_t gc9a01_init_cmds[] = {
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14, // ?
  GC9A01A_INREGEN1, 0,
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14, // ?
  0x84, 1, 0x40, // ?
  0x85, 1, 0xFF, // ?
  0x86, 1, 0xFF, // ?
  0x87, 1, 0xFF, // ?
  0x88, 1, 0x0A, // ?
  0x89, 1, 0x21, // ?
  0x8A, 1, 0x00, // ?
  0x8B, 1, 0x80, // ?
  0x8C, 1, 0x01, // ?
  0x8D, 1, 0x01, // ?
  0x8E, 1, 0xFF, // ?
  0x8F, 1, 0xFF, // ?
  0xB6, 2, 0x00, 0x00, // ?
  GC9A01A_MADCTL, 1, GC9A01A_MADCTL_BGR,
  GC9A01A_COLMOD, 1, 0x05,
  0x90, 4, 0x08, 0x08, 0x08, 0x08, // ?
  0xBD, 1, 0x06, // ?
  0xBC, 1, 0x00, // ?
  0xFF, 3, 0x60, 0x01, 0x04, // ?
  GC9A01A1_POWER2, 1, 0x13,
  GC9A01A1_POWER3, 1, 0x13,
  GC9A01A1_POWER4, 1, 0x22,
  0xBE, 1, 0x11, // ?
  0xE1, 2, 0x10, 0x0E, // ?
  0xDF, 3, 0x21, 0x0c, 0x02, // ?
  GC9A01A_GAMMA1, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA2, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  GC9A01A_GAMMA3, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA4, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  0xED, 2, 0x1B, 0x0B, // ?
  0xAE, 1, 0x77, // ?
  0xCD, 1, 0x63, // ?
  // Unsure what this line (from manufacturer's boilerplate code) is
  // meant to do, but users reported issues, seems to work OK without:
  //0x70, 9, 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03, // ?
  GC9A01A_FRAMERATE, 1, 0x34,
  0x62, 12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, // ?
            0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
  0x63, 12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, // ?
            0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
  0x64, 7, 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07, // ?
  0x66, 10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00, // ?
  0x67, 10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98, // ?
  0x74, 7, 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00, // ?
  0x98, 2, 0x3e, 0x07, // ?
  GC9A01A_TEON, 0,
  GC9A01A_INVON, 0,
  GC9A01A_SLPOUT, 0, // Exit sleep
  GC9A01A_DISPON, 0, // Display on
  0x00                  // End of list
};

void gc9a01_write_cmd_code(uint8_t cmd) {
    spi_wait_until_not_busy(gc9a01_spi_config.spi);
    gpio_write(false, gc9a01_spi_config.dc_pin);
    spi_write_byte(gc9a01_spi_config.spi, cmd);
    spi_wait_until_not_busy(gc9a01_spi_config.spi);
    gpio_write(true, gc9a01_spi_config.dc_pin);
}

void gc9a01_write_byte(uint8_t byte) {
    spi_write_byte(gc9a01_spi_config.spi, byte);
}

void gc9a01_write_buf(char *buf, size_t len) {
    spi_write_buf(gc9a01_spi_config.spi, buf, len);
}

void gc9a01_write_cmd(uint8_t cmd, uint8_t *args, size_t num_args) {
    gc9a01_write_cmd_code(cmd);
    for (uint8_t i = 0; i < num_args; i++) gc9a01_write_byte(*args++);
}

void gc9a01_write_color(GC9A01_Color color) {
    spi_write_word(gc9a01_spi_config.spi, (uint16_t)color);
}

void gc9a01_write_colors(GC9A01_Color *colors, size_t num_colors) {
    while (num_colors-- > 0) { spi_write_word(gc9a01_spi_config.spi,(uint16_t)*colors); colors += 2; };
}

Result gc9a01_init() {

    // initialize spi

    Result res = spi_init(&gc9a01_spi_config);
    if (res) return res;

    // send initialization commands

    uint8_t cmd, cmd_len, arg;
    uint8_t *cmds_ptr = (uint8_t *)gc9a01_init_cmds;
    while (*cmds_ptr) {
        cmd = *cmds_ptr++;

        // send command op code
        gc9a01_write_cmd_code(cmd);
        cmd_len = *cmds_ptr++;

        // send command arguments
        for (uint8_t i = 0; i < cmd_len; i++) {
            arg = *cmds_ptr++;
            gc9a01_write_byte(arg);
        }
        if (cmd == GC9A01A_SLPOUT || cmd == GC9A01A_DISPON) {
            spin(50000);
        }
    }

    return RES_OK;
}

Result gc9a01_set_frame(const GC9A01_Frame *frame)
{
    uint8_t data[4];

    uint8_t x1 = frame->x + frame->sx - 1U;
    uint8_t y1 = frame->y + frame->sy - 1U;

    data[0] = (frame->x >> 8) & 0xFF;
    data[1] = frame->x & 0xFF;
    data[2] = (x1 >> 8) & 0xFF;
    data[3] = x1 & 0xFF;
    gc9a01_write_cmd(GC9A01A_CASET, data, sizeof(data));

    data[0] = (frame->y >> 8) & 0xFF;
    data[1] = frame->y & 0xFF;
    data[2] = (y1 >> 8) & 0xFF;
    data[3] = y1 & 0xFF;
    gc9a01_write_cmd(GC9A01A_RASET, data, sizeof(data));

    return RES_OK;
}

Result gc9a01_draw_colors(GC9A01_Color const *colors, size_t num_colors, const GC9A01_Frame *frame, uint8_t scale) {
    gc9a01_set_frame(frame);
    gc9a01_write_cmd_code(GC9A01A_RAMWR);

    if (scale == 0) return RES_BAD_PARAM;
    else if (scale == 1) {
        gc9a01_write_colors(colors, num_colors);
    }
    else {
        uint8_t cols = frame->sx / scale;
        if (frame->sx % scale) cols++;
        uint8_t rows = frame->sy / scale;
        if (frame->sy % scale) rows++;

        if (num_colors < (rows * cols)) return RES_OUT_OF_BOUNDS;

        for (uint8_t r = 0; r < rows; r++) {
            for (uint8_t i = 0; i < scale; i++) {
                for (uint8_t c = 0; c < cols; c++) {
                    for (uint8_t j = 0; j < scale; j++) {
                        gc9a01_write_color(colors[r * rows + c]);
                    }
                } 
            }
        }        
    }

    return RES_OK;
}

Result gc9a01_draw_colors_from_bitmask(uint8_t const *mask, size_t len_bytes, GC9A01_Color fg_color, GC9A01_Color bg_color, const GC9A01_Frame *frame, uint8_t scale) {
    gc9a01_set_frame(frame);
    gc9a01_write_cmd_code(GC9A01A_RAMWR);

    if (scale == 0) return RES_BAD_PARAM;
    else if (scale == 1) {
        while (len_bytes-- > 0) { 
            uint8_t byte = *mask;
            for (uint8_t i = 0; i < 8; i++) {
                if ( byte & 0x1) gc9a01_write_color(fg_color); else gc9a01_write_color(bg_color);
                byte >>= 1; 
            }
            mask += 1;
        };
    }
    else {
        uint8_t cols = frame->sx / scale;
        if (frame->sx % scale) cols++;
        uint8_t rows = frame->sy / scale;
        if (frame->sy % scale) rows++;

        if (len_bytes * 8 < (rows * cols)) return RES_OUT_OF_BOUNDS;

        for (uint8_t r = 0; r < rows; r++) {
            for (uint8_t i = 0; i < scale; i++) {
                for (uint8_t c = 0; c < cols; c++) {
                    for (uint8_t j = 0; j < scale; j++) {
                        size_t color_no = r * cols + c;
                        size_t byte_no = color_no / 8;
                        size_t bit_no = color_no % 8;

                        if (mask[byte_no] & (0x1 << bit_no)) gc9a01_write_color(fg_color); else gc9a01_write_color(bg_color);
                    }
                } 
            }
        } 
    }

    return RES_OK;
}

