// Copyright 2022 PWrInSpace, Kuba

#include "LSM6DS3.h"
#include <assert.h>
#include <string.h>
#include "esp_log.h"

#define LSM6DS3_TAG "LSM6DS3"

static uint8_t create_register_CTRL1_XL(LSM6DS3_t *sensor) {
    uint8_t reg = 0;
    reg |= sensor->acc_bw;
    reg |= (sensor->acc_scale << 2);
    reg |= (sensor->acc_odr << 4);

    return reg;
}

static uint8_t create_register_CTRL2_G(LSM6DS3_t *sensor) {
    uint8_t reg = 0;
    if (sensor->gyro_scale != LSM6DS3_GYRO_125) {
        reg |= (sensor->gyro_scale << 2);
    }
    reg |= (sensor->gyro_odr << 4);

    return reg;
}

bool LSM6DS3_init(LSM6DS3_t *sensor, uint8_t addr, LSM6DS3_I2C_write write, LSM6DS3_I2C_read read) {
    assert(sensor != NULL);
    assert(write != NULL);
    assert(read != NULL);

    sensor->address = addr;
    sensor->i2c_write = write;
    sensor->i2c_read = read;

    sensor->acc_odr = LSM6DS3_ACC_ODR_1_66kHz;
    sensor->acc_scale = LSM6DS3_ACC_16G;
    sensor->acc_bw = LSM6DS3_ACC_BW_400Hz;
    sensor->gyro_scale = LSM6DS3_GYRO_1000;
    sensor->gyro_odr = LSM6DS3_GYRO_ODR_1_66kHz;

    uint8_t data = create_register_CTRL1_XL(sensor);
    if (sensor->i2c_write(sensor->address, LSM6DS3_REG_CTRL1_XL, &data, sizeof(data)) == false) {
        return false;
    }

    data = create_register_CTRL2_G(sensor);
    if (sensor->i2c_write(sensor->address, LSM6DS3_REG_CTRL2_G, &data, sizeof(data)) == false) {
        return false;
    }

    if (LSM6DS3_check_who_am_i(sensor) == false) {
        return false;
    }

    return true;
}

bool LSM6DS3_check_who_am_i(LSM6DS3_t *sensor) {
    assert(sensor != NULL);
    uint8_t data;
    if (sensor->i2c_read(sensor->address, LSM6DS3_REG_WHO_AM_I, &data, 1) == false) {
        data = 0;
    }

    return (data == 0x69);
}

bool LSM6DS3_set_acc_scale(LSM6DS3_t *sensor, LSM6DS3_ACC_SCALE scale) {
    sensor->acc_scale = scale;

    uint8_t data = create_register_CTRL1_XL(sensor);
    if (sensor->i2c_write(sensor->address, LSM6DS3_REG_CTRL1_XL, &data, sizeof(data)) == false) {
        return false;
    }

    return true;
}

static float get_acc_factor(LSM6DS3_ACC_SCALE scale) {
    switch (scale) {
        case LSM6DS3_ACC_2G:
            return 0.061;
        case LSM6DS3_ACC_4G:
            return 0.122;
        case LSM6DS3_ACC_8G:
            return 0.244;
        case LSM6DS3_ACC_16G:
            return 0.488;
        default:
            return -1;
    }
}

bool LSM6DS3_read_acc(LSM6DS3_t *sensor, LSM6DS3_acc_t *acc) {
    assert(sensor != NULL);
    assert(acc != NULL);

    uint8_t data[6] = {0};
    if (sensor->i2c_read(sensor->address, LSM6DS3_REG_OUTX_L_LX, data, sizeof(data)) == false) {
        return false;
    }

    int16_t temp;
    float acc_factor = get_acc_factor(sensor->acc_scale);
    temp = data[0];
    temp |= data[1] << 8;
    acc->x = temp * acc_factor / 1000.0;
    temp = data[2];
    temp |= data[3] << 8;
    acc->y = temp * acc_factor / 1000.0;
    temp = data[4];
    temp |= data[5] << 8;
    acc->z = temp * acc_factor / 1000.0;

    ESP_LOGI(LSM6DS3_TAG, "ACC X: %f, Y: %f, Z: %f", acc->x, acc->y, acc->z);

    return true;
}

bool LSM6DS3_set_gyro_scale(LSM6DS3_t *sensor, LSM6DS3_GYRO_SCALE scale) {
    sensor->gyro_scale = scale;

    uint8_t data = create_register_CTRL2_G(sensor);
    if (sensor->i2c_write(sensor->address, LSM6DS3_REG_CTRL2_G, &data, sizeof(data)) == false) {
        return false;
    }

    return true;
}

static float get_gyro_factor(LSM6DS3_GYRO_SCALE scale) {
    switch (scale) {
        case LSM6DS3_GYRO_125:
            return 4.375;
        case LSM6DS3_GYRO_245:
            return 8.75;
        case LSM6DS3_GYRO_500:
            return 17.50;
        case LSM6DS3_GYRO_1000:
            return 35;
        case LSM6DS3_GYRO_2000:
            return 70;
        default:
            return -1;
    }
}

bool LSM6DS3_read_gyro(LSM6DS3_t *sensor, LSM6DS3_gyro_t *gyro) {
    assert(sensor != NULL);
    assert(gyro != NULL);

    uint8_t data[6] = {0};
    if (sensor->i2c_read(sensor->address, LSM6DS3_REG_OUTX_L_G, data, sizeof(data)) == false) {
        return false;
    }

    int16_t temp;
    float gyro_factor = get_gyro_factor(sensor->gyro_scale);
    temp = data[0];
    temp |= data[1] << 8;
    gyro->x = temp * gyro_factor / 1000.0;
    temp = data[2];
    temp |= data[3] << 8;
    gyro->y = temp * gyro_factor / 1000.0;
    temp = data[4];
    temp |= data[5] << 8;
    gyro->z = temp * gyro_factor / 1000.0;

    return true;
}

bool LSM6DS3_acc_ready(LSM6DS3_t *sensor) {
    assert(sensor != NULL);
    uint8_t data = 0;

    sensor->i2c_read(sensor->address, LSM6DS3_REG_STATUS, &data, 1);

    return (data & 0x01);
}

bool LSM6DS3_gyro_ready(LSM6DS3_t *sensor) {
    assert(sensor != NULL);
    uint8_t data = 0;

    sensor->i2c_read(sensor->address, LSM6DS3_REG_STATUS, &data, 1);

    return (data & (1 << 1));
}
