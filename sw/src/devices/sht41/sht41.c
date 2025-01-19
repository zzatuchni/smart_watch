#include "sht41.h"

Result sht41_get_temp_hum_data(char *buf, SHT41_Precision prec) {
    uint8_t cmd_buf;

    if (prec == SHT41_HI_PREC)          cmd_buf = SHT41_GET_TEMP_HUM_HI_PREC_CMD;
    else if (prec == SHT41_MED_PREC)    cmd_buf = SHT41_GET_TEMP_HUM_MED_PREC_CMD;
    else if (prec == SHT41_LOW_PREC)    cmd_buf = SHT41_GET_TEMP_HUM_LOW_PREC_CMD;
    else return RES_BAD_PARAM;

    Result wr_res = i2c_write_buf(common_i2c_config.i2c, SHT41_I2C_ADDRESS, &cmd_buf, 1);

    if (wr_res) return wr_res;

    // TODO: make precise delay here when figuring out scheduler!!!
    spin(100000);

    return i2c_read_buf(common_i2c_config.i2c, SHT41_I2C_ADDRESS, buf, 6);
}