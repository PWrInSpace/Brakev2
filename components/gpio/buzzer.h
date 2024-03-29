// Copyright 2022 PWrInSpace, Kuba
#ifndef BUZZER_H
#define BUZZER_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"

bool BUZZER_init(uint8_t pin);
bool BUZZER_set_level(uint32_t state);
bool BUZZER_change_level(void);
uint32_t BUZZER_get_level(void);

#endif
