// Copyright 2022 PWrInSpace, Kuba

#include "brake_servo.h"

static struct {
    servo_t servo;
    bool initialized;
} gb = {
    .initialized = false,
};

bool BRAKE_SERVO_init(void) {
    servo_config_t config = {
        .min_pulse_width_us = BRAKE_SERVO_MIN_WIDTH,
        .max_pulse_width_us = BRAKE_SERVO_MAX_WIDTH,
        .min_degree = BRAKE_SERVO_MIN_DEGREE,
        .max_degree = BRAKE_SERVO_MAX_DEGREE,
        .pwm_pin = BRAKE_SERVO_PIN,
    };

    if (SERVO_init(&gb.servo, config) == false) {
        return false;
    }
    gb.initialized = true;
    return true;
}

bool BRAKE_SERVO_move(int16_t angle) {
    if (gb.initialized == false) {
        return false;
    }

    if (angle < BRAKE_SERVO_MIN_DEGREE || angle > BRAKE_SERVO_MAX_DEGREE) {
        return false;
    }

    return SERVO_move(&gb.servo, angle);
}
