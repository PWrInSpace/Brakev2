// Copyright 2022 PWrInSpace

#include "LPS25H.h"
#include "driver/i2c.h"
#include "unity.h"

#define SDA 21
#define SCL 22

#define TAG "LPS25"

static i2c_config_t i2c_config;
static i2c_port_t i2c_port = I2C_NUM_1;

static void initialize_i2c() {
  i2c_config.mode = I2C_MODE_MASTER;
  i2c_config.sda_io_num = 21;
  i2c_config.scl_io_num = 22;
  i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
  i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
  i2c_config.master.clk_speed = 400000;
  i2c_config.clk_flags = 0;

  if (i2c_param_config(i2c_port, &i2c_config) != ESP_OK) {
    ESP_LOGE(TAG, "I2C param configuration fail");
    TEST_FAIL();
  }
  if (i2c_driver_install(i2c_port, i2c_config.mode, 0, 0, 0) != ESP_OK) {
    ESP_LOGE(TAG, "I2C install fail");
    TEST_FAIL();
  }
}

TEST_CASE("LPS25H init test- pre defined i2c", "[LPS25H]") {
  uint8_t data[8] = {{0}};

  initialize_i2c();

  LPS25H lps;
  uint8_t lpsReg = LPS25H_REG_WHO_AM_I;
  LPS25HInit(&lps, i2c_port, LPS25H_I2C_ADDR_SA0_H, &i2c_config);
  LPS25HRegisterRead(&lps, lpsReg, data, 8);

  TEST_ASSERT_EQUAL(0xBD, data[0]);
  //   i2c_driver_delete(i2c_port);
}
