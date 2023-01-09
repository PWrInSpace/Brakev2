// Copyright 2022 PWrInSpace, Kuba
#include "rtos_tasks.h"

#define TAG "SENSORS"

void sensor_task(void *arg) {
    LSM6DS3_acc_t acc;
    while (1) {
        // ESP_LOGI(TAG, "Hello SENSOR_TASK");
        LSM6DS3_read_acc(&acc_sensor, &acc);
        ESP_LOGI(TAG, "Acceleration -> X:%f\tY:%f\tZ:%f", acc.x, acc.y, acc.z);
        esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT,
                          NULL, 0, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
