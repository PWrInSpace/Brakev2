// Copyright 2022 PWrInSpace

#include "driver/i2c.h"
#include "LSM6DS3.h"
#include "esp_log.h"
#include "unity.h"

#define TAG "LSM"

static i2c_config_t i2c_config;
static i2c_port_t i2c_port = I2C_NUM_1;
static LSM6DS3_t lsm_sensor;

static void initialize_i2c() {
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.master.clk_speed = 400000;
    i2c_config.clk_flags = 0;

    if (i2c_param_config(i2c_port, &i2c_config) != ESP_OK) {
        ESP_LOGE(TAG, "I2C param configuration fail");
        TEST_FAIL();
    }
    if (i2c_driver_install(i2c_port, i2c_config.mode, 0, 0, 0) != ESP_OK) {
        ESP_LOGE(TAG, "I2C install fail");
        TEST_FAIL();
    }
}

bool i2cread(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (i2c_master_write_read_device(i2c_port, dev_addr, &reg_addr, 1, data, len,
                                     pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

bool i2cwrite(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (i2c_master_write_to_device(i2c_port, dev_addr, data, len, pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

TEST_CASE("LSM6DS3 init - correct", "[LSM6DS3]") {
    bool res;
    // initialize_i2c();
    res = LSM6DS3_init(&lsm_sensor, 0x6B, i2cwrite, i2cread);
    TEST_ASSERT_EQUAL(res, true);
}

TEST_CASE("LSM6DS3 who am i", "[LSM6DS3]") {
    uint8_t res;
    res = LSM6DS3_check_who_am_i(&lsm_sensor);
    TEST_ASSERT_EQUAL(true, res);
}


TEST_CASE("LSM6DS3 read acc", "[LSM6DS3]") {
    bool res;
    LSM6DS3_acc_t acc;
    res = LSM6DS3_read_acc(&lsm_sensor, &acc);
    TEST_ASSERT_EQUAL(true, res);
}

TEST_CASE("LSM6DS3 read gyro", "[LSM6DS3]") {
    bool res;
    LSM6DS3_gyro_t gyro;
    res = LSM6DS3_read_gyro(&lsm_sensor, &gyro);
    TEST_ASSERT_EQUAL(true, res);
}

// TEST_CASE("LSM6DS3 read temperature", "[LSM6DS3]") {
//     bool res;
//     float temperature;
//     res = LSM6DS3_read_temperature(&lsm_sensor, &temperature);
//     TEST_ASSERT_EQUAL(true, res);
//     TEST_ASSERT_LESS_OR_EQUAL(70, temperature);
//     TEST_ASSERT_GREATER_OR_EQUAL(-70, temperature);
// }
