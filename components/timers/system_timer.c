// Copyright 2022 PWrInSpace, Kuba
#include <string.h>
#include <esp_log.h>
#include "system_timer.h"

static struct {
    esp_timer_handle_t timers[TIMER_MAX];
}tm;


void TIMER_init(void) {
    for (int i = 0; i < TIMER_MAX; ++i) {
        tm.timers[i] = NULL;
    }
    esp_timer_init();
}


TimerResult TIMER_start(timer_id id, uint32_t miliseconds, TimerType type,
    timer_callback callback, void*arg) {
    if (id >= TIMER_MAX) {
        return TIMER_ID_ERROR;
    }

    if (callback == NULL) {
        return TIMER_NULL_CALLBACK;
    }

    // delete previous timer
    if (tm.timers[id] != NULL) {
        TIMER_stop_and_delete(id);
    }

    esp_timer_create_args_t timer_arg = {
        .callback = callback,
        .arg = arg,
    };

    esp_timer_create(&timer_arg, &tm.timers[id]);

    if (type == TIMER_ONE_SHOT) {
        esp_timer_start_once(tm.timers[id], miliseconds * 1000);
    } else {
        esp_timer_start_periodic(tm.timers[id], miliseconds * 1000);
    }

    return TIMER_OK;
}

TimerResult TIMER_stop_and_delete(timer_id id) {
    if (id >= TIMER_MAX) {
        return TIMER_ID_ERROR;
    }

    if (tm.timers[id] != NULL) {
        esp_timer_stop(tm.timers[id]);
        esp_timer_delete(tm.timers[id]);
        tm.timers[id] = NULL;
    }
    return TIMER_OK;
}
