// Copyright 2022 PWrInSpace, Kuba

#ifndef LSM6D3_H
#define LSM6D3_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define LSM6DS3_REG_WHO_AM_I           0x0F
#define LSM6DS3_REG_OUT_TEMP_L         0x20
#define LSM6DS3_REG_OUTX_L_G           0x22
#define LSM6DS3_REG_OUTX_L_LX          0x28
#define LSM6DS3_REG_CTRL1_XL           0x10
#define LSM6DS3_REG_CTRL2_G            0x11
#define LSM6DS3_REG_CTRL6_C            0x15
#define LSM6DS3_REG_CTRL7_G            0x16
#define LSM6DS3_REG_CTRL8_XL           0x17
#define LSM6DS3_REG_STATUS             0x1E
#define LSM6DS3_REG_CTRL2_G            0x11
#define LSM6DS3_REG_CTRL3_C            0x12

typedef bool (*LSM6DS3_I2C_read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
typedef bool(*LSM6DS3_I2C_write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);

typedef enum {
    LSM6DS3_ACC_2G = 0,
    LSM6DS3_ACC_4G = 2,
    LSM6DS3_ACC_8G = 3,
    LSM6DS3_ACC_16G = 1,
} LSM6DS3_ACC_SCALE;


typedef enum {
    LSM6DS3_ACC_ODR_POWER_DOWN = 0b0000,
    LSM6DS3_ACC_ODR_12_5Hz = 0b0001,
    LSM6DS3_ACC_ODR_26Hz = 0b0010,
    LSM6DS3_ACC_ODR_52Hz = 0b0011,
    LSM6DS3_ACC_ODR_104Hz = 0b0100,
    LSM6DS3_ACC_ODR_208Hz = 0b0101,
    LSM6DS3_ACC_ODR_416Hz = 0b0110,
    LSM6DS3_ACC_ODR_833Hz = 0b0111,
    LSM6DS3_ACC_ODR_1_66kHz = 0b1000,
    LSM6DS3_ACC_ODR_3_33kHz = 0b1001,
    LSM6DS3_ACC_ODR_6_66kHz = 0b1010,
} LSM6DS3_ACC_ODR;

typedef enum {
    LSM6DS3_ACC_BW_400Hz = 0b00,
    LSM6DS3_ACC_BW_200Hz = 0b01,
    LSM6DS3_ACC_BW_100Hz = 0b10,
    LSM6DS3_ACC_BW_50Hz = 0b11,
} LSM6DS3_ACC_BW;

typedef enum {
    LSM6DS3_GYRO_125 = 0b111,
    LSM6DS3_GYRO_245 =  0b00,
    LSM6DS3_GYRO_500 = 0b01,
    LSM6DS3_GYRO_1000 = 0b10,
    LSM6DS3_GYRO_2000 = 0b11
} LSM6DS3_GYRO_SCALE;

typedef enum {
    LSM6DS3_GYRO_ODR_POWER_DOWN = 0b0000,
    LSM6DS3_GYRO_ODR_12_5Hz = 0b0001,
    LSM6DS3_GYRO_ODR_26Hz = 0b0010,
    LSM6DS3_GYRO_ODR_52Hz = 0b0011,
    LSM6DS3_GYRO_ODR_104Hz = 0b0100,
    LSM6DS3_GYRO_ODR_208Hz = 0b0101,
    LSM6DS3_GYRO_ODR_416Hz = 0b0110,
    LSM6DS3_GYRO_ODR_833Hz = 0b0111,
    LSM6DS3_GYRO_ODR_1_66kHz = 0b1000,
} LSM6DS3_GYRO_ODR;

typedef struct {
    uint8_t address;
    LSM6DS3_I2C_write i2c_write;
    LSM6DS3_I2C_read i2c_read;
    LSM6DS3_ACC_ODR acc_odr;
    LSM6DS3_ACC_SCALE acc_scale;
    LSM6DS3_ACC_BW acc_bw;
    LSM6DS3_GYRO_SCALE gyro_scale;
    LSM6DS3_GYRO_ODR gyro_odr;
} LSM6DS3_t;

typedef struct {
    float x;
    float y;
    float z;
} LSM6DS3_acc_t, LSM6DS3_gyro_t;


bool LSM6DS3_init(LSM6DS3_t *sensor, uint8_t addr, LSM6DS3_I2C_write write, LSM6DS3_I2C_read read);
bool LSM6DS3_check_who_am_i(LSM6DS3_t *sensor);
bool LSM6DS3_read_acc(LSM6DS3_t *sensor, LSM6DS3_acc_t *acc);
bool LSM6DS3_set_acc_scale(LSM6DS3_t *sensor, LSM6DS3_ACC_SCALE scale);
bool LSM6DS3_set_gyro_scale(LSM6S3_t *sensor, LSM6DS3_GYRO_SCALE scale);
bool LSM6DS3_read_gyro(LSM6DS3_t *sensor, LSM6DS3_gyro_t *gyro);
bool LSM6DS3_read_temperature(LSM6DS3_t *sensor, float *temperature);
bool LSM6DS3_acc_ready(LSM6DS3_t *sensor);
bool LSM6DS3_gyro_ready(LSM6DS3_t *sensor);
bool LSM6DS3_temperature_ready(LSM6DS3_t *sensor);

#endif
