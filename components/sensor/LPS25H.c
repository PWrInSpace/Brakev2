// Copyright 2022 PWrInSpace, Krzysztof Gliwiński

#include "LPS25H.h"

LPS25HResult LPS25HInit(LPS25H *lps, i2c_port_t portNum, uint8_t i2cAddress,
                        i2c_config_t *conf) {
  lps->port = portNum;
  lps->addr = i2cAddress;
  lps->conf = *conf;
  return LPS25H_OK;
}

LPS25HResult LPS25HRegisterRead(LPS25H *lps, uint8_t regAddr, uint8_t *data,
                                size_t len) {
  return i2c_master_write_read_device(
      lps->port, lps->addr, &regAddr, 1, data, len,
      I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

LPS25HResult LPS25HRegisterWriteByte(LPS25H *lps, uint8_t regAddr,
                                     uint8_t data) {
  uint8_t writeBuf[2] = {regAddr, data};
  return i2c_master_write_to_device(lps->port, lps->addr, writeBuf,
                                    sizeof(writeBuf),
                                    I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}
