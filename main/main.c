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
  // ESP_ERROR_CHECK(i2c_master_init());
  LPS25H lps;
  LPS25H_REG reg = WHO_AM_I;
  ESP_ERROR_CHECK(LPS25HInit(&lps, SDA, SCL, I2C_NUM_1, LPS25H_I2C_ADDR_SA0_H));
  vTaskDelay(300 / portTICK_PERIOD_MS);
  while (1) {
    LPS25HRegisterRead(&lps, reg, data, 8);
    ESP_LOGI(TAG, "Who am I?: %X\n", data[0]);
    i++;
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
