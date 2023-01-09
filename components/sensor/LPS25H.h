// Copyright 2022 PWrInSpace, Krzysztof Gliwiński

#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

#define LPS_TAG "LPS25H Error"
/*!
 \brief LPS25H_I2C_ADDR_SA0_L
      sensor I2C address if SA0 is pulled low
 */
#define LPS25H_I2C_ADDR_SA0_L 0b1011100
/*!
 \brief LPS25H_I2C_ADDR_SA0_H
      sensor I2C address if SA0 is pulled high
*/
#define LPS25H_I2C_ADDR_SA0_H 0b1011101

/// Pressure reference values
#define LPS25H_REG_REF_P_XL 0x08
#define LPS25H_REG_REF_P_L 0x09
#define LPS25H_REG_REF_P_H 0x0A
/// Who am I - returns 0xBD
#define LPS25H_REG_WHO_AM_I 0x0F
/// Internal average values register
#define LPS25H_REG_RES_CONF 0x10
/// Control registers
#define LPS25H_REG_CTRL_REG1 0x20
#define LPS25H_REG_CTRL_REG2 0x21
#define LPS25H_REG_CTRL_REG3 0x22
#define LPS25H_REG_CTRL_REG4 0x23
#define LPS25H_REG_INTERRUPT_CFG 0x24
#define LPS25H_REG_INT_SOURCE 0x25
/// Status register -
#define LPS25H_REG_STATUS_REG 0x00
/// Output pressure registers
#define LPS25H_REG_PRESS_OUT_XL 0x28
#define LPS25H_REG_PRESS_OUT_L 0x29
#define LPS25H_REG_PRESS_OUT_H 0x2A
/// Temperature output values
#define LPS25H_REG_TEMP_OUT_L 0x2B
#define LPS25H_REG_TEMP_OUT_H 0x2C
/// FIFO functionality
#define LPS25H_REG_FIFO_CTRL 0x2E
#define LPS25H_REG_FIFO_STATUS 0x2F
/// Threshold values for pressure interrupt generation
#define LPS25H_REG_THS_P_L 0x30
#define LPS25H_REG_THS_P_H 0x31

#define I2C_MASTER_TIMEOUT_MS 1000

typedef enum{
  LPS25H_OK = 0,
  LPS25H_ReadError,
  LPS25H_WriteError
} LPS25HResult;

typedef struct {
  i2c_config_t conf;
  i2c_port_t port;
  uint8_t addr;
} LPS25H;

/*!
  \brief Initialize the sensor. \n
  if conf == 0 setup the i2c alongside sensor variables
  \return LPS25H_OK
*/
LPS25HResult LPS25HInit(LPS25H *lps, i2c_port_t portNum,
                     uint8_t i2cAddress, i2c_config_t *conf);
/*!
  \brief Read sensor register of length len
  \return ESP_OK if the read is successful, ESP_FAIL otherwise
*/
LPS25HResult LPS25HRegisterRead(LPS25H *lps, uint8_t regAddr, uint8_t *data,
                             size_t len);

/*!
  \brief Write to the sensor register
  \return ESP_OK if the write is successful, ESP_FAIL otherwise
*/
LPS25HResult LPS25HRegisterWriteByte(LPS25H *lps, uint8_t regAddr, uint8_t data);
