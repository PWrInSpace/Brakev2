// Copyright 2022 PWrInSpace, Kuba

#include "timers_callbacks.h"
#include "igniter.h"
#include "buzzer.h"
#include "brake_servo.h"
#include "recovery_servo.h"
#include "settings.h"
#include "rtos_tasks.h"
#include "esp_log.h"

#define TAG "DUPA"

void TIMER_CB_brake_open(void *arg) {
    esp_event_post_to(event_get_handle(), TASK_EVENTS, BRAKE_OPEN_EVENT,
                      NULL, 0, portMAX_DELAY);
}

void TIMER_CB_brake_close(void *arg) {
    BRAKE_SERVO_move(SETI_get_settings()->brake_close_angle);
}

void TIMER_CB_recov_trig(void *arg) {
    esp_event_post_to(event_get_handle(), TASK_EVENTS, APOGEE_EVENT,
                      NULL, 0, portMAX_DELAY);
}

void TIMER_CB_recov_close(void *arg) {
    RECOV_SERVO_move(SETI_get_settings()->recovery_close_angle);
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
