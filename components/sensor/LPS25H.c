// Copyright 2022 PWrInSpace

#include "LPS25H.h"

esp_err_t LPS25HInit(LPS25H *lps, int sda, int scl, i2c_port_t portNum,
                     uint8_t i2cAddress) {
  lps->port = portNum;
  lps->conf.mode = I2C_MODE_MASTER;
  lps->conf.sda_io_num = sda;
  lps->conf.scl_io_num = scl;
  lps->conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  lps->conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  lps->conf.master.clk_speed = 400000;
  lps->conf.clk_flags = 0;
  if (i2c_param_config(lps->port, &lps->conf) == ESP_ERR_INVALID_ARG) {
    return ESP_FAIL;
  }
  lps->addr = i2cAddress;
  return i2c_driver_install(lps->port, lps->conf.mode, 0, 0, 0);
}

esp_err_t LPS25HRegisterRead(LPS25H *lps, uint8_t regAddr, uint8_t *data,
                             size_t len) {
  return i2c_master_write_read_device(
      lps->port, lps->addr, &regAddr, 1, data, len,
      I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}
