// Copyright 2022 PWrInSpace, Kuba

#include "recovery_servo.h"

static struct {
    servo_t servo;
    bool initialized;
} gb = {
    .initialized = false,
};

bool RECOV_SERVO_init(void) {
    servo_config_t config = {
        .min_pulse_width_us = RECOV_SERVO_MIN_WIDTH,
        .max_pulse_width_us = RECOV_SERVO_MAX_WIDTH,
        .min_degree = RECOV_SERVO_MIN_DEGREE,
        .max_degree = RECOV_SERVO_MAX_DEGREE,
        .pwm_pin = RECOV_SERVO_PIN,
    };

    if (SERVO_init(&gb.servo, config) == false) {
        return false;
    }
    gb.initialized = true;
    return true;
}

bool RECOV_SERVO_move(int16_t angle) {
    if (gb.initialized == false) {
        return false;
    }

    if (angle < RECOV_SERVO_MIN_DEGREE || angle > RECOV_SERVO_MAX_DEGREE) {
        return false;
    }

    return SERVO_move(&gb.servo, angle);
}
