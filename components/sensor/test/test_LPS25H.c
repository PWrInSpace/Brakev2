// Copyright 2022 PWrInSpace

#include "LPS25H.h"
#include "unity.h"

#define SDA 21
#define SCL 22

TEST_CASE("LPS25H init test", "[LPS25H]") {
  uint8_t data[8];
  // ESP_ERROR_CHECK(i2c_master_init());
  LPS25H lps;
  LPS25H_REG reg = WHO_AM_I;
  ESP_ERROR_CHECK(LPS25HInit(&lps, SDA, SCL, I2C_NUM_1, LPS25H_I2C_ADDR_SA0_H));
  vTaskDelay(300 / portTICK_PERIOD_MS);

  LPS25HRegisterRead(&lps, reg, data, 8);

  TEST_ASSERT_EQUAL(0xBD, data[0]);
}
