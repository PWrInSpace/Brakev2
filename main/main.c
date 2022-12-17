// Copyright 2022 PWrInSpace

#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "voltageMeasure.h"
#include "LPS25H.h"

#define TAG "MAIN"

#define SDA 21
#define SCL 22

void app_main(void) {
  int i = 0;
  // voltageMeasure vBat = {.channel = ADC1_CHANNEL_4,
  //                        .adcRaw = 0,
  //                        .voltage = 0,
  //                        .voltageDivider = 3.319727891};
  // voltageMeasureInit(&vBat);
    uint8_t data[8];
  //   // ESP_ERROR_CHECK(i2c_master_init());
  //   LPS25H lps;
  //   LPS25H_REG lpsReg = WHO_AM_I;

  //   ESP_ERROR_CHECK(LPS25HInit(&lps, SDA, SCL, I2C_NUM_1,
  //   LPS25H_I2C_ADDR_SA0_H, NULL)); vTaskDelay(300 / portTICK_PERIOD_MS);
  //   while (1) {
  //     LPS25HRegisterRead(&lps, lpsReg, data, 8);
  //     ESP_LOGI(TAG, "Who am I?: %X\n", data[0]);
  //     i++;
  //     vTaskDelay(500 / portTICK_PERIOD_MS);
  //   }
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
  LPS25H_REG lpsReg = WHO_AM_I;
  ESP_ERROR_CHECK(LPS25HInit(&lps, 0, 0, port, LPS25H_I2C_ADDR_SA0_H, &master));
  while (1) {
      LPS25HRegisterRead(&lps, lpsReg, data, 8);
      ESP_LOGI(TAG, "Who am I?: %X\n", data[0]);
      i++;
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
