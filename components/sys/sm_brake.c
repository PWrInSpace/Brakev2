// Copyright 2022 PWrInSpace, Kuba
#include "sm_brake.h"
#include "buzzer.h"
#include "timers_callbacks.h"

void SM_ascent_cb(void *arg) {
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 500, TIMER_PERIODIC, TIMER_CB_buzzer_change, NULL);
}

void SM_brake_cb(void *arg) {
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 250, TIMER_PERIODIC, TIMER_CB_buzzer_change, NULL);
}

void SM_descent_cb(void *arg) {
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 500, TIMER_PERIODIC, TIMER_CB_buzzer_change, NULL);
}

void SM_ground_cb(void *arg) {
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 1000, TIMER_PERIODIC, TIMER_CB_buzzer_change, NULL);
}

