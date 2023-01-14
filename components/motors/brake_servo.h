// Copyright 2022 PWrInSpace, Kuba

#ifndef BRAKE_SERVO_H
#define BRAKE_SERVO_H

#include <stdbool.h>
#include <stdint.h>
#include "servo.h"
#include "config.h"

#define BRAKE_SERVO_MIN_WIDTH 900
#define BRAKE_SERVO_MAX_WIDTH 2100
#define BRAKE_SERVO_MIN_DEGREE 0
#define BRAKE_SERVO_MAX_DEGREE 120
#define BRAKE_SERVO_PIN PCB_SERVO3_PWM

bool BRAKE_SERVO_init(void);
bool BRAKE_SERVO_move(int16_t angle);

#endif
