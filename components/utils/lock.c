// Copyright 2022 PWrInSpace, Kuba
#include "lock.h"

#define LOCK_NULL_NB 0xFF

static struct {
    struct {
        uint8_t id;
        bool released;
    } locks[MAX_LOCKS_NUMBER];
    uint8_t number_of_locks;
    SemaphoreHandle_t mutex;
} gb = {
    .mutex = NULL,
};

bool LOCK_init(void) {
    gb.mutex = xSemaphoreCreateMutex();
    if (gb.mutex == NULL) {
        return false;
    }
    memset(&gb.locks, 0, sizeof(gb.locks));

    return true;
}

bool LOCK_create(uint8_t lock_id, bool acquire) {
    if (gb.number_of_locks >= MAX_LOCKS_NUMBER) {
        return false;
    }

    gb.locks[gb.number_of_locks].id = lock_id;

    if (acquire == true) {
        gb.locks[gb.number_of_locks].released = true;
    } else {
        gb.locks[gb.number_of_locks].released = false;
    }

    return true;
}

static uint8_t get_lock_position(uint8_t lock_id) {
    for (int i = 0; i < MAX_LOCKS_NUMBER; ++i) {
        if (gb.locks[i].id == lock_id) {
            return i;
        }
    }

    return MAX_LOCKS_NUMBER;
}

bool LOCK_release(uint8_t lock_id) {
    uint8_t pos = get_lock_position(lock_id);
    if (pos == MAX_LOCKS_NUMBER) {
        return false;
    }

    xSemaphoreTake(gb.mutex, portMAX_DELAY);
    gb.locks[pos].released = true;
    xSemaphoreGive(gb.mutex);
    return true;
}
bool LOCK_acquire(uint8_t lock_id) {
    uint8_t pos = get_lock_position(lock_id);
    if (pos == MAX_LOCKS_NUMBER) {
        return false;
    }

    xSemaphoreTake(gb.mutex, portMAX_DELAY);
    gb.locks[pos].released = false;
    xSemaphoreGive(gb.mutex);
    return true;
}

LOCK_STATUS LOCK_status(uint8_t lock_id) {
    uint8_t pos = get_lock_position(lock_id);
    if (pos == MAX_LOCKS_NUMBER) {
        return LOCK_ERROR;
    }

    xSemaphoreTake(gb.mutex, portMAX_DELAY);
    bool released = gb.locks[pos].released = false;
    xSemaphoreGive(gb.mutex);
    return released == true ? LOCK_RELEASED : LOCK_LOCKED;
}
