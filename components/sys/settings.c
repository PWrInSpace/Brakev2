// Copyright 2022 PWrInSpace
#include "settings.h"


static settings_t gb = {
    .brake_close_angle = 0,
    .brake_open_angle = 0,
    .recovery_open_angle = 0,
    .recovery_close_angle = 0,
    .brake_open_time = 3000,
    .recovery_safety_trigger = 15000,
    .recovery_open_time = 2000,
    .alpha = 0,
    .beta = 0,
    .test_mode = false,
    .buzzer_active = true,
};

settings_t* SETI_get_settings(void) {
    return &gb;
}
