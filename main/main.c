// Copyright 2022 PWrInSpace

#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "voltageMeasure.h"
#include "LPS25H.h"
#include "LSM6DS3.h"

#define TAG "MAIN"

#define SDA 21
#define SCL 22

static i2c_config_t i2c_config;

static esp_err_t i2c_sensor_init(void) {
    esp_err_t res;
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.master.clk_speed = 400000;
    i2c_config.clk_flags = 0;

    res = i2c_param_config(I2C_NUM_1, &i2c_config);
    ESP_ERROR_CHECK(res);
    res = i2c_driver_install(I2C_NUM_1, i2c_config.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);
    return res;
}

static bool i2c_num1_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (i2c_master_write_read_device(I2C_NUM_1, dev_addr, &reg_addr, 1, data, len,
                                     pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

static bool i2c_num1_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (i2c_master_write_to_device(I2C_NUM_1, dev_addr, data, len, pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

void app_main(void) {
  LSM6DS3_t sensor;
  LSM6DS3_acc_t acc;
  LSM6DS3_gyro_t gyro;
  float temperature;

  i2c_sensor_init();
  LSM6DS3_init(&sensor, 0x6B, i2c_num1_write, i2c_num1_read);

  while (1) {
    LSM6DS3_acc_ready(&sensor);
    LSM6DS3_read_acc(&sensor, &acc);
    // LSM6DS3_read_gyro(&sensor, &gyro);
    LSM6DS3_read_temperature(&sensor, &temperature);

    // ESP_LOGI(TAG, "ACC: X %f\tY %f\t Z %f", acc.x, acc.y, acc.z);
    // ESP_LOGI(TAG, "GYRO: X %f\tY %f\t Z %f", gyro.x, gyro.y, gyro.z);
    // ESP_LOGI(TAG, "Temperature %f", temperature);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
