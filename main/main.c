// Copyright 2022 PWrInSpace

#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LPS25H.h"
#include "LSM6DS3.h"
#include "sdcard.h"
#include "config.h"

#define TAG "MAIN"

#define SDA 21
#define SCL 22

static i2c_config_t i2c_config;
static spi_bus_config_t spi_bus;
static spi_host_device_t spi_host;

static esp_err_t spi_init(void) {
    esp_err_t ret;
    spi_bus.mosi_io_num = PCB_MOSI,
    spi_bus.miso_io_num = PCB_MISO,
    spi_bus.sclk_io_num = PCB_SCK,
    spi_bus.quadwp_io_num = -1,
    spi_bus.quadhd_io_num = -1,
    spi_bus.max_transfer_sz = 4000,
    spi_host = HSPI_HOST;

    ret = spi_bus_initialize(spi_host, &spi_bus, SDSPI_DEFAULT_DMA);
    return ret;
}

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

void app_main(void) {

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
