// Copyright 2022 PWrInSpace, Kuba
#include <math.h>

#include "rtos_tasks.h"
#include "utils.h"
#define TAG "SENSORS"

static struct {
  alphaBetaValues height;
  struct {
    alphaBetaValues x;
    alphaBetaValues y;
    alphaBetaValues z;
  } acc;
  struct {
    alphaBetaValues x;
    alphaBetaValues y;
    alphaBetaValues z;
  } gyro;
} alpha_beta_filters;

static struct {
  led_driver_t x;
  led_driver_t y;
  led_driver_t z;
} acc_leds;

static sensors_t brake_sensors;

static bool filter_init(void) {
  bool res = 1;
  res &= alphaBetaInit(&alpha_beta_filters.height, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.acc.x, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.acc.y, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.acc.z, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.gyro.x, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.gyro.y, 0.3, 0.05);
  res &= alphaBetaInit(&alpha_beta_filters.gyro.z, 0.3, 0.05);
  return res;
}

static bool acc_leds_init(void) {
  bool res = 1;
  res &= led_driver_init(&acc_leds.x, PCB_LED_X, LEDC_CHANNEL_0, LEDC_TIMER_0);
  res &= led_driver_init(&acc_leds.y, PCB_LED_Y, LEDC_CHANNEL_1, LEDC_TIMER_1);
  res &= led_driver_init(&acc_leds.z, PCB_LED_Z, LEDC_CHANNEL_2, LEDC_TIMER_2);
  if (!res) {
    ESP_LOGE(TAG, "Accelerometer leds driver init failed");
  }
  return res;
}

#define CRIT_LED_VAL 2.f

static uint16_t get_duty_acc(float acc) {
  if (fabsf(acc) >= CRIT_LED_VAL) {
    return MAX_LED_DUTY;
  }
  return (uint16_t)(fabsf(acc) / CRIT_LED_VAL * (float)MAX_LED_DUTY);
}

static bool acc_leds_update(void) {
  bool res = 1;
  res &= led_driver_update_duty_cycle(&acc_leds.x,
                                      get_duty_acc(brake_sensors.acc.x));
  res &= led_driver_update_duty_cycle(&acc_leds.y,
                                      get_duty_acc(brake_sensors.acc.y));
  res &= led_driver_update_duty_cycle(&acc_leds.z,
                                      get_duty_acc(brake_sensors.acc.z));
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

  brake_sensors.filtered_data.gyro.x = alphaBetaFilter(
      &alpha_beta_filters.gyro.x, brake_sensors.gyro.x, up_time);
  brake_sensors.filtered_data.gyro.y = alphaBetaFilter(
      &alpha_beta_filters.gyro.y, brake_sensors.gyro.y, up_time);
  brake_sensors.filtered_data.gyro.z = alphaBetaFilter(
      &alpha_beta_filters.gyro.z, brake_sensors.gyro.z, up_time);
  ESP_LOGI(TAG, "Filtered gyro: x: %f\t y:%f\tz:%f",
           brake_sensors.filtered_data.gyro.x,
           brake_sensors.filtered_data.gyro.y,
           brake_sensors.filtered_data.gyro.z);
}

void sensor_task(void *arg) {
  if (!filter_init()) {
    ESP_LOGE(TAG, "Alpha-beta filters not initiated!");
  }
  if (!acc_leds_init()) {
    ESP_LOGE(TAG, "Acceleration sensor leds not initiated!");
  }
  while (1) {
    LSM6DS3_read_acc(&acc_sensor, &brake_sensors.acc);

    ESP_LOGI(TAG, "Acceleration -> X: %f\tY: %f\tZ: %f", brake_sensors.acc.x,
             brake_sensors.acc.y, brake_sensors.acc.z);

    LSM6DS3_read_gyro(&acc_sensor, &brake_sensors.gyro);

    ESP_LOGI(TAG, "Gyroscope -> X: %f\tY: %f\tZ: %f", brake_sensors.gyro.x,
             brake_sensors.gyro.y, brake_sensors.gyro.z);

    LPS25HGetHeightAndPressure(&press_sensor, &brake_sensors.height,
                               &brake_sensors.pressure);
    LPS25HReadTemperature(&press_sensor, &brake_sensors.temp);

    ESP_LOGI(TAG, "LPS25H data -> Height: %f\tPressure: %f\tTemperature %f",
             brake_sensors.height, brake_sensors.pressure, brake_sensors.temp);

    brake_sensors.vBatt = voltageMeasureReadVoltage(&vMes);

    ESP_LOGI(TAG, "Battery voltage -> %f V", brake_sensors.vBatt);

    // Filters
    filtered_update();
    // LEDs
    acc_leds_update();

    esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT,
                      (void *)&brake_sensors, sizeof(sensors_t), portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
