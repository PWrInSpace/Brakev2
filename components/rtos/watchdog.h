// Copyright 2023 PWr in Space
#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define WATCHDOG_MAX_TASKS 16

#ifndef WATCHDOG_MAX_TASKS
#warning \
    "Watchdog max tasks undefined"
#endif

typedef void (*watchdog_callback_t)(TaskHandle_t);

void watchdog_background_task(void* arg);
void watchdog_init(size_t delay, size_t watchdog_stack_size,
                   size_t watchdog_task_priority, watchdog_callback_t callback);
void watchdog_disconnect();
void watchdog_reconnect(size_t delay, size_t watchdog_task_stack_size,
                        size_t watchdog_task_priority,
                        watchdog_callback_t callback);
int watchdog_check_all();
bool watchdog_append(TaskHandle_t feeder);
bool watchdog_pop(TaskHandle_t feeder);
int watchdog_feed(TaskHandle_t feeder);
int watchdog_check(TaskHandle_t feeder);
int watchdog_starve(TaskHandle_t feeder);
