// Copyright 2022 PWrInSpace, Kuba

#ifndef TIMERS_CALLBACKS_H
#define TIMERS_CALLBACKS_H

#include "system_timer.h"

void TIMER_CB_brake_open(void *arg);
void TIMER_CB_brake_close(void *arg);
void TIMER_CB_igniter_high(void *arg);
void TIMER_CB_igniter_low(void *arg);
void TIMER_CB_buzzer_change(void *arg);
void TIMER_CB_buzzer_high(void *arg);
void TIMER_CB_buzzer_low(void *arg);
void TIMER_CB_recov_trig(void *arg);
void TIMER_CB_recov_close(void *arg);

#endif
