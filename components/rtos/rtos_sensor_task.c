// Copyright 2022 PWrInSpace, Kuba
#include "rtos_tasks.h"

#define TAG "SENSORS"

static sensors_t brake_sensors;

void sensor_task(void *arg) {
  while (1) {
    LSM6DS3_read_acc(&acc_sensor, &brake_sensors.acc);

    ESP_LOGI(TAG, "Acceleration -> X: %f\tY: %f\tZ: %f", brake_sensors.acc.x,
             brake_sensors.acc.y, brake_sensors.acc.z);

    LPS25HGetHeightAndPressure(&press_sensor, &brake_sensors.height,
                               &brake_sensors.pressure);
    LPS25HReadTemperature(&press_sensor, &brake_sensors.temp);

    ESP_LOGI(TAG, "LPS25H data -> Height: %f\tPressure: %f\tTemperature %f",
             brake_sensors.height, brake_sensors.pressure, brake_sensors.temp);

    brake_sensors.vBatt = voltageMeasureReadVoltage(&vMes);

    ESP_LOGI(TAG, "Battery voltage -> %f V", brake_sensors.vBatt);

    esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT,
                      (void *)&brake_sensors, sizeof(sensors_t), portMAX_DELAY);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
