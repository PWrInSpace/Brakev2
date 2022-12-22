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
  while(1){
    ESP_LOGI("ITER %d", i);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
