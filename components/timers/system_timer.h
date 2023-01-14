// Copyright 2022 PWrInSpace, Kuba
#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include "esp_timer.h"

#define TIMER_MAX 15

typedef enum {
    BRAKE_TIMER = 0,
    IGNITER_TIMER,
    BUZZER_TIMER,
} Timers;

typedef enum {
    TIMER_OK = 0,
    TIMER_MEMORY_ERROR,
    TIMER_ID_ERROR,
    TIMER_UNKNOWN_TYPE,
    TIMER_NULL_CALLBACK,
}TimerResult;

typedef enum {
    TIMER_ONE_SHOT = 0,
    TIMER_PERIODIC,
}TimerType;

typedef uint8_t timer_id;

typedef void (*timer_callback)(void *arg);

void TIMER_init(void);
TimerResult TIMER_start(timer_id id, uint32_t miliseconds, TimerType type,
    timer_callback callback, void*arg);
TimerResult TIMER_stop_and_delete(timer_id id);

#endif
