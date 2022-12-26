// Copyright 2022 PWrInSpace

#include <stdio.h>

#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "voltageMeasure.h"

#include "esp_event.h"

#define TAG "MAIN"

esp_event_loop_handle_t event_loop_handle;

void app_main(void) {
    esp_event_loop_args_t event_loop = {
        .queue_size = 5,
        .task_name = "loop_task",
        .task_priority = 10,
        .task_stack_size = 3072,
        .task_core_id = PRO_CPU_NUM,
    };

    esp_event_handler_register_with()
}
