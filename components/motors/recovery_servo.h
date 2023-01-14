// Copyright 2022 PWrInSpace, Kuba

#ifndef RECOV_SERVO_H
#define RECOV_SERVO_H

#include <stdbool.h>
#include <stdint.h>
#include "servo.h"
#include "config.h"

#define RECOV_SERVO_MIN_WIDTH 900
#define RECOV_SERVO_MAX_WIDTH 2100
#define RECOV_SERVO_MIN_DEGREE 0
#define RECOV_SERVO_MAX_DEGREE 120
#define RECOV_SERVO_PIN PCB_SERVO2_PWM

bool RECOV_SERVO_init(void);
bool RECOV_SERVO_move(int16_t angle);

#endif
