#ifndef BREJK_SYSTEM_H
#define BREJK_SYSTEM_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2c.h"
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "rtos_tasks.h"

typedef enum {
    SAVE_TO_SD,
    SAVE_TO_FLASH,
    SAVE_TO_BOTH,
} DATA_SAVE_OPTIONS;

typedef struct {
    TaskHandle_t sensor_task;
    TaskHandle_t main_task;
    TaskHandle_t memory_task;
    TaskHandle_t test_mode_task;
    QueueHandle_t data_to_memory;
} rtos_t;

typedef struct {
    spi_bus_config_t spi_bus;
    spi_host_device_t spi_host;
} spi_t;

typedef struct {
    i2c_config_t i2c_config;
} i2c_t;

typedef struct {
    uint8_t todo;
} sensors_t;

typedef struct {
    int64_t up_time;
    uint8_t state;
    sensors_t sensors_data;
} rocket_data_t;

typedef struct {
    rocket_data_t data;
    DATA_SAVE_OPTIONS save_option;
} data_to_memory_task_t;

bool rtos_init(void);
bool rtos_test_mode_init(void);
rtos_t* get_rtos_struct(void);

bool spi_init(void);
spi_t* get_spi_struct(void);

bool i2c_sensor_init(void);
bool i2c_num1_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
bool i2c_num1_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
i2c_t* get_i2c_struct(void);

#endif