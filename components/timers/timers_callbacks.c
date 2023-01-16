// Copyright 2022 PWrInSpace, Kuba

#include "timers_callbacks.h"
#include "igniter.h"
#include "buzzer.h"
// #include "brake_servo.h"
// #include "recovery_servo.h"

void TIMER_CB_brake_open(void *arg) {
    // TO DO XDDDDDDDD
}

void TIMER_CB_brake_close(void *arg) {
    // TO DO XDDD
}

void TIMER_CB_igniter_high(void *arg) {
    IGNITER_set_level(1);
}

void TIMER_CB_igniter_low(void *arg) {
    IGNITER_set_level(0);
}

void TIMER_CB_buzzer_change(void *arg) {
    BUZZER_change_level();
}

void TIMER_CB_buzzer_high(void *arg) {
    BUZZER_set_level(1);
}

void TIMER_CB_buzzer_low(void *arg) {
    BUZZER_set_level(0);
}
