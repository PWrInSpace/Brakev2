// Copyright 2022 PWrInSpace, Kuba
#include "buzzer.h"

static uint8_t gb_pin;

bool BUZZER_init(uint8_t pin) {
    gb_pin = pin;
    if (gpio_set_direction(gb_pin, GPIO_MODE_OUTPUT) != ESP_OK) {
        return false;
    }
    return true;
}

bool BUZZER_set_level(uint32_t state) {
    if (gpio_set_level(gb_pin, state) != ESP_OK) {
        return false;
    }

    return true;
}

bool BUZZER_change_level(void) {
    uint32_t level = BUZZER_get_level();
    if (gpio_set_level(gb_pin, level == BUZZER_HIGH ? BUZZER_LOW : BUZZER_HIGH) != ESP_OK) {
        return false;
    }

    return true;
}

uint32_t BUZZER_get_level(void) {
    return gpio_get_level(gb_pin) != 0 ? BUZZER_HIGH : BUZZER_LOW;
}
