#include "lis2mdl.h"

char mgnmtr_data_buf[6] = {0};

Result lis2mdl_calibrate() {

    Result wr_res;
    uint8_t cmd_buf[2];

    cmd_buf[0] = LIS2MDL_OFFSETZ_L_REG_ADDRESS;
    cmd_buf[1] = (uint8_t)(LIS2MDL_OFFSETZ_VALUE & 0xFF);
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    cmd_buf[0] = LIS2MDL_OFFSETZ_H_REG_ADDRESS;
    cmd_buf[1] = (uint8_t)(LIS2MDL_OFFSETZ_VALUE >> 8);
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    cmd_buf[0] = LIS2MDL_OFFSETX_L_REG_ADDRESS;
    cmd_buf[1] = (uint8_t)(LIS2MDL_OFFSETX_VALUE & 0xFF);
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    cmd_buf[0] = LIS2MDL_OFFSETX_H_REG_ADDRESS;
    cmd_buf[1] = (uint8_t)(LIS2MDL_OFFSETX_VALUE >> 8);
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    cmd_buf[0] = LIS2MDL_OFFSETY_L_REG_ADDRESS;
    cmd_buf[1] = (uint8_t)(LIS2MDL_OFFSETY_VALUE & 0xFF);
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    cmd_buf[0] = LIS2MDL_OFFSETY_H_REG_ADDRESS;
    cmd_buf[1] = (uint8_t)(LIS2MDL_OFFSETY_VALUE >> 8);
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

}

Result lis2mdl_init() {
    uint8_t cmd_buf[2] = {LIS2MDL_CFG_REG_A_ADDRESS, LIS2MDL_CFG_REG_A_STARTUP_VALUE};

    // Perform startup sequence
    Result wr_res = lis2mdl_calibrate();
    if (wr_res) return wr_res;


    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    cmd_buf[0] = LIS2MDL_CFG_REG_C_ADDRESS;
    cmd_buf[1] = LIS2MDL_CFG_REG_C_STARTUP_VALUE;

    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 2);
    if (wr_res) return wr_res;

    return RES_OK;
}

Result lis2mdl_get_raw_data(Magnetometer_Raw_Data *mgnmtr_data) {
    Result wr_res;
    uint8_t cmd_buf;

    cmd_buf = LIS2MDL_OUTX_L_REG_ADDRESS;
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 1);
    if (wr_res) return wr_res;
    //spin(100000);
    wr_res = i2c_read_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, mgnmtr_data_buf, 1);
    if (wr_res) return wr_res;

    cmd_buf = LIS2MDL_OUTX_H_REG_ADDRESS;
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 1);
    if (wr_res) return wr_res;
    //spin(100000);
    wr_res = i2c_read_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, mgnmtr_data_buf+1, 1);
    if (wr_res) return wr_res;

    cmd_buf = LIS2MDL_OUTY_L_REG_ADDRESS;
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 1);
    if (wr_res) return wr_res;
    //spin(100000);
    wr_res = i2c_read_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, mgnmtr_data_buf+2, 1);
    if (wr_res) return wr_res;

    cmd_buf = LIS2MDL_OUTY_H_REG_ADDRESS;
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 1);
    if (wr_res) return wr_res;
    //spin(100000);
    wr_res = i2c_read_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, mgnmtr_data_buf+3, 1);
    if (wr_res) return wr_res;

    cmd_buf = LIS2MDL_OUTZ_L_REG_ADDRESS;
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 1);
    if (wr_res) return wr_res;
    //spin(100000);
    wr_res = i2c_read_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, mgnmtr_data_buf+4, 1);
    if (wr_res) return wr_res;

    cmd_buf = LIS2MDL_OUTZ_H_REG_ADDRESS;
    wr_res = i2c_write_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, (char *)&cmd_buf, 1);
    if (wr_res) return wr_res;
    //spin(100000);
    wr_res = i2c_read_buf(common_i2c_config.i2c, LIS2MDL_I2C_ADDRESS, mgnmtr_data_buf+5, 1);
    if (wr_res) return wr_res;

    mgnmtr_data->x = (int16_t)((uint16_t)(mgnmtr_data_buf[0]) | (uint16_t)(mgnmtr_data_buf[1] << 8));
    mgnmtr_data->y = (int16_t)((uint16_t)(mgnmtr_data_buf[2]) | (uint16_t)(mgnmtr_data_buf[3] << 8));
    mgnmtr_data->z = (int16_t)((uint16_t)(mgnmtr_data_buf[4]) | (uint16_t)(mgnmtr_data_buf[5] << 8));

    return RES_OK;
}

const int16_t piecewise_a[17] = {10, 9, 8, 7, 6,  5,  4,  3,  2,  2,  2,  2,  2,  2,  2,  2,  2};
const int16_t piecewise_b[17] = {0,  2, 4, 7, 12, 17, 23, 31, 40, 40, 41, 41, 41, 40, 40, 39, 39};

int16_t arctan(int16_t x) {
    // we will do a piecewise linear function for convenience
    DPRINTN(x);
    DPRINTNL();
    int16_t res;
    int16_t neg = 0;
    if (x < 0) { x *= -1; neg = 180; }
    
    if (x >= 7500) res = 90;
    else if (x >= 3500) res = 89;
    else if (x >= 1700) res = 88;
    else if (x >= 1100) res = 87;
    else if (x >= 830) res = 86;
    else if (x >= 300) res = (x / 100) + 78;
    else if (x >= 200) res = (x / 20) + 65;
    else if (x >= 170) res = (x / 10) + 55;
    else {
        int16_t ind = x / 10;
        res = ((x * piecewise_a[ind]) / 10) + piecewise_b[ind];
    }
    DPRINTN(res);
    DPRINTNL();

    return (res + neg) % 360;
}

int16_t lis2mdl_get_heading_degrees(int16_t x, int16_t y) {
    if (x > 0) return arctan(y/x);
    else if (x < 0 && y >= 0) return arctan(y/x)+180;
    else if (x < 0 && y < 0) return arctan(y/x)-180;
    else if (x == 0 && y > 0) return 90;
    else if (x == 0 && y < 0) return 270;
    return -1;
}