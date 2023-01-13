// Copyright 2022 PWrInSpace

#include "rtos_tasks.h"
#include "watchdog.h"
#include "esp_log.h"

#define TAG "MAIN"

static watchdog_callback_t watchdogHandle;

void app_main(void) {
    ESP_LOGI(TAG, "CREATING INIT TASK");
    if (xTaskCreate(init_task, "rtos init", 8000, NULL, 0, NULL) != pdPASS) {
        ESP_LOGE(TAG, "UNABLE TO CREATE INIT TASK");
    }
    ESP_LOGI(TAG, "CREATING WATCHDOG TASK");
    watchdog_init(100, 8000, TASK_PRIORITY_HIGH, watchdogHandle);
    vTaskDelete(NULL);
}
