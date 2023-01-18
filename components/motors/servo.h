// Copyright 2022 PWrInSpace

#ifndef SERVO_H
#define SERVO_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/mcpwm_prelude.h"

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

typedef struct {
    uint16_t min_pulse_width_us;
    uint16_t max_pulse_width_us;
    int16_t min_degree;
    int16_t max_degree;
    uint8_t pwm_pin;
} servo_config_t;


typedef struct {
    servo_config_t config;
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t operator;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
} servo_t;

bool SERVO_init(servo_t *servo, servo_config_t config);

bool SERVO_move(servo_t *servo, int16_t angle);

// bool SERVO_deinit()

#endif
