// Copyright 2022 PWrInSpace
#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <stdint.h>


typedef struct {
    uint16_t brake_close_angle;
    uint16_t brake_open_angle;
    uint16_t recovery_open_angle;
    uint16_t recovery_close_angle;
    uint16_t brake_open_time;
    uint16_t recovery_safety_trigger;
    uint16_t recovery_open_time;
    float alpha;
    float beta;
    bool test_mode;
    bool buzzer_active;
} settings_t;

settings_t* SETI_get_settings(void);

#endif
