// Copyright 2022 PWrInSpace, Kuba
#include "buzzer.h"
#include "esp_log.h"
#define TAG "DUPO"

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
    uint32_t new_level = BUZZER_get_level() == 0 ? 1 : 0;
    if (gpio_set_level(gb_pin, new_level) != ESP_OK) {
        return false;
    }

    return true;
}

uint32_t BUZZER_get_level(void) {
    gpio_set_direction(gb_pin, GPIO_MODE_INPUT);
    uint32_t level = gpio_get_level(gb_pin);
    gpio_set_direction(gb_pin, GPIO_MODE_OUTPUT);
    return level;
}
