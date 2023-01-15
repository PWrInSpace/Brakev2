// Copyright 2022 PWrInSpace, Kuba
// Copyright 2022 PWrInSpace, Kuba
#ifndef LOCK_H
#define LOCK_H

#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define MAX_LOCKS_NUMBER 10

typedef enum {
    LOCK_RELEASED,
    LOCK_LOCKED,
    LOCK_ERROR,
} LOCK_STATUS;

bool LOCK_init(void);
bool LOCK_create(uint8_t lock_id, bool acquire);
bool LOCK_release(uint8_t lock_id);
bool LOCK_acquire(uint8_t lock_id);
LOCK_STATUS LOCK_status(uint8_t lock_id);

#endif

