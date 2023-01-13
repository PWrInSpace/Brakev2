// Copyright 2022 PWrInSpace, Krzysztof Gliwiński

#include "LPS25H.h"

LPS25HResult LPS25HInit(LPS25H *lps, i2c_port_t portNum, uint8_t i2cAddress) {
  lps->port = portNum;
  lps->addr = i2cAddress;
  lps->fifoConfigured = false;
  ESP_LOGI(LPS_TAG, "Sensor initiated");
  return LPS25H_OK;
}

LPS25HResult LPS25HRegisterRead(LPS25H *lps, uint8_t regAddr, uint8_t *data,
                                size_t len) {
  return i2c_master_write_read_device(
             lps->port, lps->addr, &regAddr, 1, data, len,
             I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS) == ESP_OK
             ? LPS25H_OK
             : LPS25H_WriteError;
}

LPS25HResult LPS25HRegisterWriteByte(LPS25H *lps, uint8_t regAddr,
                                     uint8_t data) {
  uint8_t writeBuf[2] = {regAddr, data};
  if (i2c_master_write_to_device(
          lps->port, lps->addr, writeBuf, sizeof(writeBuf),
          I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS) == ESP_FAIL) {
    ESP_LOGE(LPS_TAG, "I2C write failed");
    return LPS25H_WriteError;
  }
  return LPS25H_OK;
}

LPS25HResult LPS25HStdConf(LPS25H *lps) {
  LPS25HResult res = LPS25H_OK;
  res |= LPS25HRegisterWriteByte(lps, LPS25H_REG_RES_CONF, 0x05);
  res |= LPS25HRegisterWriteByte(lps, LPS25H_REG_FIFO_CTRL, 0xDF);
  res |= LPS25HRegisterWriteByte(lps, LPS25H_REG_CTRL_REG2, 0x40);
  res |= LPS25HRegisterWriteByte(lps, LPS25H_REG_CTRL_REG1, 0xA0);
  if (res == LPS25H_OK) {
    lps->fifoConfigured = true;
    ESP_LOGI(LPS_TAG, "Sensor fifo mode configured (data rate 7Hz)");
    return LPS25H_OK;
  }
  ESP_LOGE(LPS_TAG, "Sensor fifo mode configuration failed");
  return LPS25H_ConfigError;
}

LPS25HResult LPS25HReadPressure(LPS25H *lps, float *pressureVal) {
  if (!lps->fifoConfigured) {
    ESP_LOGE(LPS_TAG, "Sensor fifo mode not configured!");
    return LPS25H_ConfigError;
  }

  LPS25HResult res = LPS25H_OK;
  uint8_t pressureRaw[3] = {0};
  int32_t pressureProcessed;
  // ADDR | 0x80 sets auto increment
  res |=
      LPS25HRegisterRead(lps, LPS25H_REG_PRESS_OUT_XL | 0x80, pressureRaw, 3);
  pressureProcessed = (int32_t)pressureRaw[2];
  pressureProcessed <<= 8;
  pressureProcessed |= (int32_t)pressureRaw[1];
  pressureProcessed <<= 8;
  pressureProcessed |= (int32_t)pressureRaw[0];
  ESP_LOGI(LPS_TAG, "Raw pressure value: %ld, pressure in bytes %d %d %d",
           pressureProcessed, pressureRaw[2], pressureRaw[1], pressureRaw[0]);
  *pressureVal = ((float)pressureProcessed / 4096.f);
  return res == LPS25H_OK ? LPS25H_OK : LPS25H_ReadError;
}

LPS25HResult LPS25HReadTemperature(LPS25H *lps, float *tempVal) {
  if (!lps->fifoConfigured) {
    ESP_LOGE(LPS_TAG, "Sensor fifo mode not configured!");
    return LPS25H_ConfigError;
  }

  LPS25HResult res = LPS25H_OK;
  uint8_t tempRaw[2] = {0};
  res |= LPS25HRegisterRead(lps, LPS25H_REG_TEMP_OUT_L | 0x80, tempRaw, 2);
  int16_t tempProcessed;
  tempProcessed = (int16_t)tempRaw[1];
  tempProcessed <<= 8;
  tempProcessed |= (int16_t)tempRaw[0];
  ESP_LOGI(LPS_TAG, "Raw temp value: %d, temp in bytes %d %d ", tempProcessed,
           tempRaw[1], tempRaw[0]);
  *tempVal = ((float)tempProcessed / 480.f + 42.5f);
  return res == LPS25H_OK ? LPS25H_OK : LPS25H_ReadError;
}

LPS25HResult LPS25HGetHeightAndPressure(LPS25H *lps, float *height,
                                        float *press) {
  LPS25HResult res = LPS25H_OK;
  res |= LPS25HReadPressure(lps, press);
  if (res != LPS25H_OK) {
    *height = 0;
    return LPS25H_ReadError;
  }
  *height = 44330 * (1 - pow((*press / REFERENCE_PRESSURE_HPA), 1.f / 5.255f));
  return LPS25H_OK;
}
