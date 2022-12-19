// Copyright 2022 PWrInSpace

#include "LPS25H.h"
#include "unity.h"

#define SDA 21
#define SCL 22

TEST_CASE("LPS25H init test", "[LPS25H]") {
  uint8_t data[8];
  // ESP_ERROR_CHECK(i2c_master_init());
  LPS25H lps;
  uint8_t reg = LPS25H_REG_WHO_AM_I;
  ESP_ERROR_CHECK(
      LPS25HInit(&lps, SDA, SCL, I2C_NUM_1, LPS25H_I2C_ADDR_SA0_H, NULL));
  vTaskDelay(300 / portTICK_PERIOD_MS);

  LPS25HRegisterRead(&lps, reg, data, 8);

  TEST_ASSERT_EQUAL(0xBD, data[0]);
}

TEST_CASE("LPS25H init test 2 - pre defined i2c", "[LPS25H]") {
  uint8_t data[8];
  // ESP_ERROR_CHECK(i2c_master_init());
  i2c_config_t master;
  i2c_port_t port = I2C_NUM_1;

  master.mode = I2C_MODE_MASTER;
  master.sda_io_num = 21;
  master.scl_io_num = 22;
  master.sda_pullup_en = GPIO_PULLUP_ENABLE;
  master.scl_pullup_en = GPIO_PULLUP_ENABLE;
  master.master.clk_speed = 400000;
  master.clk_flags = 0;

  i2c_param_config(port, &master);
  i2c_driver_install(port, master.mode, 0, 0, 0);
  LPS25H lps;
  uint8_t reg = LPS25H_REG_WHO_AM_I;
  ESP_ERROR_CHECK(LPS25HInit(&lps, 0, 0, port, LPS25H_I2C_ADDR_SA0_H, &master));
  vTaskDelay(300 / portTICK_PERIOD_MS);

  LPS25HRegisterRead(&lps, reg, data, 8);

  TEST_ASSERT_EQUAL(0xBD, data[0]);
}
