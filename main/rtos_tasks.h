#ifndef ROTS_TASK_H
#define RTOS_TASK_H

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_event_base.h"

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

ESP_EVENT_DECLARE_BASE(TASK_EVENTS);
bool event_loop_init(void);
bool event_loop_register(void);
esp_event_loop_handle_t event_get_handle(void);

void memory_task(void *arg);
void main_task(void *arg);
void sensor_task(void *arg);
void test_mode_task(void *arg);

#endif