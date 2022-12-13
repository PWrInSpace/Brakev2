// Copyright 2022 PWrInSpace

#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "driver/i2c.h"

/*!
 \def LPS25H_I2C_ADDR_SA0_L
      sensor I2C address if SA0 is pulled low
 */
/*!
 \def LPS25H_I2C_ADDR_SA0_H
      sensor I2C address if SA0 is pulled high
*/
#define LPS25H_I2C_ADDR_SA0_L 0b1011100
#define LPS25H_I2C_ADDR_SA0_H 0b1011101

/*!
  \enum LPS25H_REG
        addresses of LPS25H sensor registers
*/
typedef enum {
  REF_P_XL = 0x08,
  REF_P_L = 0x09,
  REF_P_H = 0x0A,
  WHO_AM_I = 0x0F,
  RES_CONF = 0x10,
  CTRL_REG1 = 0x20,
  CTRL_REG2 = 0x21,
  CTRL_REG3 = 0x22,
  CTRL_REG4 = 0x23,
  INTERRUPT_CFG = 0x24,
  INT_SOURCE = 0x25,
  STATUS_REG = 0x00,
  PRESS_OUT_XL = 0x28,
  PRESS_OUT_L = 0x29,
  PRESS_OUT_H = 0x2A,
  /// Temperature output values
  TEMP_OUT_L = 0x2B,
  TEMP_OUT_H = 0x2C,
  /// FIFO functionality
  FIFO_CTRL = 0x2E,
  FIFO_STATUS = 0x2F,
  /// Threshold values for pressure interrupt generation
  THS_P_L = 0x30,
  THS_P_H = 0x31
} LPS25H_REG;

#define I2C_MASTER_TIMEOUT_MS 1000

typedef struct {
  i2c_config_t conf;
  i2c_port_t port;
  uint8_t addr;
} LPS25H;

esp_err_t LPS25HInit(LPS25H *lps, int sda, int scl, i2c_port_t portNum,
                     uint8_t i2cAddress);

esp_err_t LPS25HRegisterRead(LPS25H *lps, uint8_t regAddr, uint8_t *data,
                             size_t len);