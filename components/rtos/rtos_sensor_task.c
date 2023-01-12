// Copyright 2022 PWrInSpace, Kuba
#include "rtos_tasks.h"

#define TAG "SENSORS"

static sensors_t sensors;

void sensor_task(void *arg) {
  float height;
  while (1) {
    LSM6DS3_read_acc(&acc_sensor, &sensors.acc);
    
    ESP_LOGI(TAG, "Acceleration -> X:%f\tY:%f\tZ:%f", sensors.acc.x,
             sensors.acc.y, sensors.acc.z);

    esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT,
                      (void *)&sensors, sizeof(sensors_t), portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
