// Copyright 2022 PWrInSpace, Kuba

#ifndef ROTS_TASK_H
#define RTOS_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "spi.h"
#include "i2c.h"
#include "LPS25H.h"
#include "LSM6DS3.h"
#include "sdcard.h"


typedef enum {
    SAVE_DATA_EVENT,
    SENSORS_NEW_DATA_EVENT,
    SENSORS_HIGH_ACC_EVENT,
    SENSORS_APOGEE_EVENT,
} EVENTS;

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

extern rtos_t rtos;
extern spi_t sd_spi;
extern i2c_t i2c_sensors;
extern sd_card_t sd_card;
extern LSM6DS3_t acc_sensor;

ESP_EVENT_DECLARE_BASE(TASK_EVENTS);
bool event_loop_init(void);
bool event_loop_register(void);
esp_event_loop_handle_t event_get_handle(void);

bool rtos_init(void);

void init_task(void *arg);
void memory_task(void *arg);
void main_task(void *arg);
void sensor_task(void *arg);
void test_mode_task(void *arg);

#endif
