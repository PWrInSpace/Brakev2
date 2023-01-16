// Copyright 2022 PWrInSpace, Kuba
#include "buzzer.h"
#include "esp_log.h"
#define TAG "DUPO"

static struct {
    uint8_t pin;
    bool on;
} gb;

bool BUZZER_init(uint8_t pin) {
    gb.pin = pin;
    gb.on = false;
    if (gpio_set_direction(gb.pin, GPIO_MODE_OUTPUT) != ESP_OK) {
        return false;
    }

    if (gpio_set_level(gb.pin, gb.on) == false) {
        return false;
    }

    return true;
}

bool BUZZER_set_level(uint32_t state) {
    gb.on = state > 0 ? true : false;
    if (gpio_set_level(gb.pin, gb.on) != ESP_OK) {
        return false;
    }

    return true;
}


bool BUZZER_change_level(void) {
    gb.on = !gb.on;
    if (gpio_set_level(gb.pin, gb.on) != ESP_OK) {
        return false;
    }

    return true;
}

uint32_t BUZZER_get_level(void) {
    return gb.on;
}
