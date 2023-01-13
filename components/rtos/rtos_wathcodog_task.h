#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#ifndef WATCHDOG_MAX_TASKS
#warning "To define the max amount of tasks the watchdog should track, define the WATCHDOG_MAX_TASKS macro"
#define WATCHDOG_MAX_TASKS 16
#endif

typedef void(*watchdog_callback_t)(TaskHandle_t);

void watchdog_background_task(void* arg);
void watchdog_init(size_t delay, size_t watchdog_stack_size, size_t watchdog_task_priority, watchdog_callback_t callback);
void watchdog_disconnect();
void watchdog_reconnect();
int watchdog_check_all();
bool watchdog_append(TaskHandle_t feeder);
bool watchdog_pop(TaskHandle_t feeder);
int watchdog_feed(TaskHandle_t feeder);
int watchdog_check(TaskHandle_t feeder);
int watchdog_starve(TaskHandle_t feeder);
