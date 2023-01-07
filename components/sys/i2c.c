// Copyright 2022 PWrInSpace, Kuba
#include "i2c.h"

bool I2C_init(i2c_t *i2c, i2c_port_t port, uint8_t sda, uint8_t scl) {
    esp_err_t res;
    i2c->mode = I2C_MODE_MASTER;
    i2c->sda_io_num = sda;
    i2c->scl_io_num = scl;
    i2c->sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c->scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c->master.clk_speed = 400000;
    i2c->clk_flags = 0;

    res = i2c_param_config(port, i2c);
    res = i2c_driver_install(port, i2c->mode, 0, 0, 0);
    return res == ESP_OK ? true : false;
}
