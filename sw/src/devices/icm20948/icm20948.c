#include "icm20948.h"

Result icm20948_init() {

    Result wr_res;
    uint8_t cmd_buf;
    uint8_t res_buf;

    for (;;) {

        cmd_buf = ICM_BANK_SELECT_REG_ADDRESS;
        wr_res = i2c_write_buf(common_i2c_config.i2c, ICM_I2C_ADDRESS, (char *)&cmd_buf, 1);
        if (wr_res) return wr_res;
        spin(100000);
        wr_res = i2c_read_buf(common_i2c_config.i2c, ICM_I2C_ADDRESS, res_buf, 1);
        if (wr_res) return wr_res;
        spin(100000);

        DPRINTN(res_buf);

    }


    return RES_OK;
}

