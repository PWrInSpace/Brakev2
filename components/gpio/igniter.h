// Copyright 2022 PWrInSpace, Kuba
#ifndef IGNITER_H
#define IGNITER_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"

bool IGNITER_init(uint8_t pin);
bool IGNITER_set_level(uint32_t state);

#endif
