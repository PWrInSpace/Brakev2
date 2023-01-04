// Copyright 2022 PWrInSpace

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "console.h"
#include "console_commands.h"
#include "state_machine.h"
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
    assert(len == 1);
    uint8_t tmp[2] = {reg_addr, data[0]};
    if (i2c_master_write_to_device(I2C_NUM_1, dev_addr, tmp, sizeof(tmp),
        pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

esp_console_cmd_t console_commands[] = {
    {"test", "test1234", NULL, CLI_test, NULL},
    {"sm-state", "Get current state", NULL, CLI_state_machine_get_state, NULL},
};


void app_main(void) {
    int i = 0;
    console_init();
    console_register_commands(console_commands,
        sizeof(console_commands)/sizeof(console_commands[0]));
    SM_init();

    while (1) {
        ESP_LOGI(TAG, "[%d] Hello world!\n", i);
        i++;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
