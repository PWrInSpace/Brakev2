// Copyright 2022 PWrInSpace

#include "LPS25H.h"

void LPS25HInit(LPS25H *lps, uint8_t sda, uint8_t scl, i2c_port_t portNum) {
  lps->port = portNum;
  lps->conf.mode = I2C_MODE_MASTER;
  lps->conf.sda_io_num = sda;
  lps->conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  lps->conf.scl_io_num = scl;
  lps->conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  lps->conf.master.clk_speed = 100000;
  lps->conf.clk_flags = 0;
  i2c_param_config(lps->port, &lps->conf);
  ESP_ERROR_CHECK(i2c_driver_install(lps->port, I2C_MODE_MASTER, 0, 0, 0));
}
