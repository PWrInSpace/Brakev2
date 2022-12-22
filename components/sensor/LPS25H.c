// Copyright 2022 PWrInSpace, Krzysztof Gliwiński

#include "LPS25H.h"

esp_err_t LPS25HInit(LPS25H *lps, int sda, int scl, i2c_port_t portNum,
                     uint8_t i2cAddress, i2c_config_t *conf) {
  lps->port = portNum;
  lps->addr = i2cAddress;
  if ((void *)conf == NULL) {
    lps->conf.mode = I2C_MODE_MASTER;
    lps->conf.sda_io_num = sda;
    lps->conf.scl_io_num = scl;
    lps->conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    lps->conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    lps->conf.master.clk_speed = 400000;
    lps->conf.clk_flags = 0;
    if (i2c_param_config(lps->port, &lps->conf) == ESP_ERR_INVALID_ARG) {
      ESP_LOGE(LPS_TAG, "I2C Param Config initialization Error!");
      return ESP_FAIL;
    }
    if (i2c_driver_install(lps->port, lps->conf.mode, 0, 0, 0) == ESP_FAIL) {
      ESP_LOGE(LPS_TAG, "I2C driver install Error!");
      return ESP_FAIL;
    } else {
      return ESP_OK;
    }
  } else {
    lps->conf = *conf;
  }
  return ESP_OK;
}

esp_err_t LPS25HRegisterRead(LPS25H *lps, uint8_t regAddr, uint8_t *data,
                             size_t len) {
  return i2c_master_write_read_device(
      lps->port, lps->addr, &regAddr, 1, data, len,
      I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

esp_err_t LPS25HRegisterWriteByte(LPS25H *lps, uint8_t regAddr, uint8_t data) {
  uint8_t writeBuf[2] = {regAddr, data};
  return i2c_master_write_to_device(lps->port, lps->addr, writeBuf,
                                    sizeof(writeBuf),
                                    I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}
