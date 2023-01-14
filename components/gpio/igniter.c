// Copyright 2022 PWrInSpace, Kuba
#include "igniter.h"

static uint8_t gb_pin;

bool IGNITER_init(uint8_t pin) {
    gb_pin = pin;
    if (gpio_set_direction(gb_pin, GPIO_MODE_OUTPUT) != ESP_OK) {
        return false;
    }
    return true;
}

bool IGNITER_set_level(uint32_t state) {
    if (gpio_set_level(gb_pin, state) != ESP_OK) {
        return false;
    }

    return true;
}
