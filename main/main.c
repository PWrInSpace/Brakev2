// Copyright 2022 PWrInSpace

#include "esp_log.h"
#include "rtos_tasks.h"

#define TAG "MAIN"

void app_main(void) {
  ESP_LOGI(TAG, "CREATING INIT TASK");
  if (xTaskCreate(init_task, "rtos init", 8000, NULL, 0, NULL) != pdPASS) {
    ESP_LOGE(TAG, "UNABLE TO CREATE INIT TASK");
  }
  ESP_LOGI(TAG, "CREATING WATCHDOG TASK");

  vTaskDelete(NULL);
}
