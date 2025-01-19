#include "sht41.h"

char temp_hum_buf[6] = {0};

Result sht41_get_temp_hum_data(Temp_Humidity_Data *th_data, SHT41_Precision prec) {
    uint8_t cmd_buf;

    if (prec == SHT41_HI_PREC)          cmd_buf = SHT41_GET_TEMP_HUM_HI_PREC_CMD;
    else if (prec == SHT41_MED_PREC)    cmd_buf = SHT41_GET_TEMP_HUM_MED_PREC_CMD;
    else if (prec == SHT41_LOW_PREC)    cmd_buf = SHT41_GET_TEMP_HUM_LOW_PREC_CMD;
    else return RES_BAD_PARAM;

    Result wr_res = i2c_write_buf(common_i2c_config.i2c, SHT41_I2C_ADDRESS, &cmd_buf, 1);

    if (wr_res) return wr_res;

    // TODO: make precise delay here when figuring out scheduler!!!
    spin(100000);

    wr_res = i2c_read_buf(common_i2c_config.i2c, SHT41_I2C_ADDRESS, temp_hum_buf, 6);

    if (wr_res) return wr_res;

    uint16_t t_ticks = temp_hum_buf[0] * 256 + temp_hum_buf[1];
    uint16_t rh_ticks = temp_hum_buf[3] * 256 + temp_hum_buf[4];
    th_data->t_degC = -45 + 175 * t_ticks/65535;
    th_data->rh_pRH = -6 + 125 * rh_ticks/65535;
    if (th_data->rh_pRH > 100)
        th_data->rh_pRH = 100;
    if (th_data->rh_pRH < 0)
        th_data->rh_pRH = 0;

    return RES_OK;
}