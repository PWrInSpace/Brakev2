// Copyright 2022 PWrInSpace, Kuba
#include "rtos_tasks.h"
#include "utils.h"
#define TAG "SENSORS"

struct {
  alphaBetaValues height;
  struct {
    alphaBetaValues x;
    alphaBetaValues y;
    alphaBetaValues z;
  } acc;
} alpha_beta_filters;

static sensors_t brake_sensors;

static bool filter_init(void) {
  bool res = 1;
  res &= alphaBetaInit(&alpha_beta_filters.height, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.acc.x, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.acc.y, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.acc.z, 0.3, 0.05);
  return res;
}

static void filtered_update() {
  float up_time = (float)get_time_ms() * 1000.f;

  brake_sensors.filtered_data.height = alphaBetaFilter(
      &alpha_beta_filters.height, brake_sensors.height, up_time);
  ESP_LOGI(TAG, "Filtered height: %f", brake_sensors.filtered_data.height);
  brake_sensors.filtered_data.acc.x =
      alphaBetaFilter(&alpha_beta_filters.acc.x, brake_sensors.acc.x, up_time);
  brake_sensors.filtered_data.acc.y =
      alphaBetaFilter(&alpha_beta_filters.acc.y, brake_sensors.acc.y, up_time);
  brake_sensors.filtered_data.acc.z =
      alphaBetaFilter(&alpha_beta_filters.acc.z, brake_sensors.acc.z, up_time);
  ESP_LOGI(TAG, "Filtered acceleration: x: %f\t y:%f\tz:%f",
           brake_sensors.filtered_data.acc.x, brake_sensors.filtered_data.acc.y,
           brake_sensors.filtered_data.acc.z);
}

void sensor_task(void *arg) {
  if (!filter_init()) {
    ESP_LOGE(TAG, "Alpha-beta filters not initiated!");
  }
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

    // Filters
    filtered_update();

    esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT,
                      (void *)&brake_sensors, sizeof(sensors_t), portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
