// Copyright 2022 PWrInSpace, Kuba
#ifndef SM_CALLBACK_H
#define SM_CALLBACK_H

#include "state_machine.h"

typedef enum {
    LAUNCHPAD,
    ASCENT,
    BRAKE,
    DESCENT,
    GROUND,
} STATES;

// void SM_launchpad_cb(void *arg);
void SM_ascent_cb(void *arg);
void SM_brake_cb(void *arg);
void SM_descent_cb(void *arg);
void SM_ground_cb(void *arg);

#endif
