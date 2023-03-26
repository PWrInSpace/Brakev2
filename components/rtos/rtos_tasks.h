// Copyright 2022 PWrInSpace

#ifndef ROTS_TASK_H
#define RTOS_TASK_H

#include "LPS25H.h"
#include "LSM6DS3.h"
#include "alpha_beta_filter.h"
#include "brake_servo.h"
#include "buzzer.h"
#include "config.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "flash_nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"
#include "igniter.h"
#include "recovery_servo.h"
#include "sdcard.h"
#include "spi.h"
#include "timers_callbacks.h"
#include "uart.h"
#include "voltageMeasure.h"
#include "state_machine.h"
#include "watchdog.h"
#include "settings.h"
#include "led_driver.h"
#include "flight_time.h"

// NVS KEYS
#define NVS_BRAKE_OPEN_ANGLE        "B_OPEN_ANG"
#define NVS_BRAKE_CLOSE_ANGLE       "B_CLOSE_ANG"
#define NVS_RECOVERY_OPEN_ANGLE     "R_OPEN_ANG"
#define NVS_RECOVERY_CLOSE_ANGLE    "R_CLOSE_ANG"
#define NVS_ALPHA                   "ALPHA_F"
#define NVS_BETA                    "BETA_F"
#define NVS_TEST_MODE               "TEST_MODE"
#define NVS_BUZZER_ACTIVE           "BUZZ_ACTIVE"
#define NVS_BRAKE_OPEN_TIME         "B_OPEN_TIME"
#define NVS_RECOV_SAFETY_TRIG_TIME  "R_SAFE_TRIG"
#define NVS_RECOV_OPEN_TIME         "R_OPEN_TIME"

#define RTOS_FLASH_QUEUE_DATA_TO_STASH 20
#define RTOS_FLASH_QUEUE_SIZE 45
#define RTOS_FLASH_SAVE 10
#define RTOS_SD_QUEUE_SIZE 30
#define RTOS_SD_SAVE 10

typedef enum {
  LAUNCHPAD,
  ASCENT,
  BRAKE,
  DESCENT,
  GROUND,
} STATES;

typedef enum {
  TEST_MODE_ON = 1,
  TEST_MODE_OFF = 0,
} TEST_MODE;

typedef enum {
  TASK_PRIORITY_LOW = 0,
  TASK_PRIORITY_MID = 5,
  TASK_PRIORITY_HIGH = 10,
} TASKS_PRIORITY;

typedef enum {
  SAVE_DATA_EVENT,
  SENSORS_NEW_DATA_EVENT,
  SENSORS_HIGH_ACC_EVENT,
  APOGEE_EVENT,
  TOUCHDOWN_EVENT,
  BRAKE_OPEN_EVENT,
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
  TaskHandle_t flash_task;
  TaskHandle_t test_mode_task;
  QueueHandle_t data_to_memory;
  QueueHandle_t data_to_flash;
} rtos_t;

typedef struct {
  LSM6DS3_acc_t acc;
  LSM6DS3_gyro_t gyro;
  float height;
  float pressure;
  float temp;
  float vBatt;
  float velocity;
  struct {
    float height;
    LSM6DS3_acc_t acc;
    LSM6DS3_gyro_t gyro;
  } filtered;
} sensors_t;

typedef struct {
  int64_t up_time;
  int64_t flight_time;
  uint8_t state;
  sensors_t sensors;
} rocket_data_t;

typedef struct {
  rocket_data_t data;
  DATA_SAVE_OPTIONS save_option;
} data_to_memory_task_t;

extern rtos_t rtos;
extern spi_t sd_spi;
extern i2c_t i2c_sensors;
extern uart_t uart;
extern sd_card_t sd_card;
extern LSM6DS3_t acc_sensor;
extern LPS25H press_sensor;
extern VoltageMeasure vMes;

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
void flash_task(void *arg);
size_t create_data_csv(rocket_data_t *rec, char *data_string, size_t len);

#endif
