// Copyright 2022 PWrInSpace, Kuba
#include <math.h>

#include "rtos_tasks.h"
#include "utils.h"
#define TAG "SENSORS"
#define LOG_LOCAL_LEVEL ESP_LOG_WARN

#define CRIT_ACCELERATION 5.f  // g
#define CRIT_TOUCHDOWN_ACC -3.f
#define CRIT_APOGEE_ZERO_G_MARGIN 0.2f  // g
#define CRIT_CONTINUOUS_NUM_ACC 5
#define CRIT_CONTINUOUS_NUM_APOGEE 5
#define CRIT_CONTINUOUS_NUM_TOUCHDOWN 3

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

static bool check_for_high_acc_event(void) {
  static uint8_t consecutive_high_acc = 0;
  if (brake_sensors.filtered.acc.z > CRIT_ACCELERATION) {
    consecutive_high_acc++;
  } else if (consecutive_high_acc != 0) {
    consecutive_high_acc--;
  }
  return consecutive_high_acc >= CRIT_CONTINUOUS_NUM_ACC;
}

static bool check_for_apogee_event(void) {
  static uint8_t consecutive_zero_g = 0;
  if (fabsf(brake_sensors.filtered.acc.x + brake_sensors.filtered.acc.y +
            brake_sensors.filtered.acc.z) < CRIT_APOGEE_ZERO_G_MARGIN &&
      alpha_beta_filters.height.vk < 0.f) {
    consecutive_zero_g++;
  } else if (consecutive_zero_g != 0) {
    consecutive_zero_g--;
  }
  return consecutive_zero_g >= CRIT_CONTINUOUS_NUM_APOGEE;
}

static bool check_for_touchdown_event(void) {
  static uint8_t consecutive_touchdown = 0;
  if (brake_sensors.filtered.acc.z < CRIT_TOUCHDOWN_ACC &&
      fabsf(alpha_beta_filters.height.vk) <= 0.5f) {
    consecutive_touchdown++;
  } else if (consecutive_touchdown != 0) {
    consecutive_touchdown--;
  }
  return consecutive_touchdown >= CRIT_CONTINUOUS_NUM_TOUCHDOWN;
}

static void filtered_update() {
  float up_time = (float)get_time_ms() * 1000.f;

  brake_sensors.filtered.height = alphaBetaFilter(
      &alpha_beta_filters.height, brake_sensors.height, up_time);
  ESP_LOGI(TAG, "Filtered height: %f", brake_sensors.filtered.height);
  brake_sensors.filtered.acc.x =
      alphaBetaFilter(&alpha_beta_filters.acc.x, brake_sensors.acc.x, up_time);
  brake_sensors.filtered.acc.y =
      alphaBetaFilter(&alpha_beta_filters.acc.y, brake_sensors.acc.y, up_time);
  brake_sensors.filtered.acc.z =
      alphaBetaFilter(&alpha_beta_filters.acc.z, brake_sensors.acc.z, up_time);
  ESP_LOGI(TAG, "Filtered acceleration: x: %f\t y:%f\tz:%f",
           brake_sensors.filtered.acc.x, brake_sensors.filtered.acc.y,
           brake_sensors.filtered.acc.z);

  brake_sensors.filtered.gyro.x = alphaBetaFilter(
      &alpha_beta_filters.gyro.x, brake_sensors.gyro.x, up_time);
  brake_sensors.filtered.gyro.y = alphaBetaFilter(
      &alpha_beta_filters.gyro.y, brake_sensors.gyro.y, up_time);
  brake_sensors.filtered.gyro.z = alphaBetaFilter(
      &alpha_beta_filters.gyro.z, brake_sensors.gyro.z, up_time);
  ESP_LOGI(TAG, "Filtered gyro: x: %f\t y:%f\tz:%f",
           brake_sensors.filtered.gyro.x, brake_sensors.filtered.gyro.y,
           brake_sensors.filtered.gyro.z);
}

#define EXIT_COMMAND "exit\r"

static bool check_if_exit_command(char* data) {
    if (strcmp(data, EXIT_COMMAND) == 0) {
        return true;
    }

    return false;
}

static void TEST_read_uart_data(void) {
    char buffer[100] = {0};
    size_t length = UART_read_until(&uart, buffer, sizeof(buffer), '\r', pdMS_TO_TICKS(500));
    if (length > 0) {
        ESP_LOGE(TAG, "Read from serial\nlength: %d\ntext: %s", length, buffer);
        if (check_if_exit_command(buffer) == true) {
            esp_restart();
        }
        if (sscanf(buffer, "%f;%f;%f;%f;%f;%f;%f;%f", &brake_sensors.acc.x, &brake_sensors.acc.y, &brake_sensors.acc.z,
                   &brake_sensors.gyro.x, &brake_sensors.gyro.y, &brake_sensors.gyro.z, &brake_sensors.pressure, &brake_sensors.temp) < 8) {
            ESP_LOGE(TAG, "READ ERRORR");
        }
    }
}

void sensor_task(void *arg) {
  if (!filter_init()) {
    ESP_LOGE(TAG, "Alpha-beta filters not initiated!");
  }
  if (!acc_leds_init()) {
    ESP_LOGE(TAG, "Acceleration sensor leds not initiated!");
  }
  while (1) {
    if (SETI_get_settings()->test_mode == true) {
        TEST_read_uart_data();
    } else {
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
    }

    brake_sensors.vBatt = voltageMeasureReadVoltage(&vMes);

    ESP_LOGI(TAG, "Battery voltage -> %f V", brake_sensors.vBatt);

    // Filters
    filtered_update();
    // LEDs
    acc_leds_update();

    if (check_for_high_acc_event()) {
      esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_HIGH_ACC_EVENT,
                        (void *)NULL, 0, portMAX_DELAY);
    }

    if (check_for_apogee_event()) {
      esp_event_post_to(event_get_handle(), TASK_EVENTS, APOGEE_EVENT,
                        (void *)NULL, 0, portMAX_DELAY);
    }

    if (check_for_touchdown_event()) {
      esp_event_post_to(event_get_handle(), TASK_EVENTS, TOUCHDOWN_EVENT,
                        (void *)NULL, 0, portMAX_DELAY);
    }

    esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT,
                      (void *)&brake_sensors, sizeof(sensors_t), portMAX_DELAY);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}
