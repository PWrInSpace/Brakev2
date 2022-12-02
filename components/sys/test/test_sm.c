// Copyright 2022 PWrInSpace

#include <stdio.h>
#include "unity.h"
#include "state_machine.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#define TEST_VALUE 10

static struct {
    int launchpad_callback;
    int flight_callback;
    int air_brake_callback;
    int apogee_callback;
    int end_test;
    int ground_callback;
}test_sm = {
    .launchpad_callback = 0,
    .flight_callback = 0,
    .air_brake_callback = 0,
    .apogee_callback = 0,
    .end_test = 0,
    .ground_callback = 0,
};

void launchpad_callback(void *arg) {
    test_sm.launchpad_callback = TEST_VALUE;
}

void flight_callback(void *arg) {
    test_sm.flight_callback = TEST_VALUE;
}

void air_brake_callback(void *arg) {
    test_sm.air_brake_callback = TEST_VALUE;
}

void apogee_callback(void *arg) {
    test_sm.apogee_callback = TEST_VALUE;
}

void on_ground_callback(void *arg) {
    test_sm.ground_callback = TEST_VALUE;
}

void end_test(void) {
    test_sm.end_test = TEST_VALUE;
}

typedef enum {
    LAUNCHPAD,
    ASCENT,
    AIRBRAKE,
    DESCENT,
    GROUND,
} states;

state_config_t states_config[] = {
    {LAUNCHPAD, launchpad_callback, NULL},
    {ASCENT, flight_callback, NULL},
    {AIRBRAKE, air_brake_callback, NULL},
    {DESCENT, apogee_callback, NULL},
    {GROUND, on_ground_callback, NULL}
};



TEST_CASE("State machine init without all states", "[sm]") {
    SM_Response err = SM_OK;
    SM_init();
    err |= SM_run();

    TEST_ASSERT_TRUE(err != SM_OK);
}

TEST_CASE("State machine init witch zero states", "[sm]") {
    SM_Response err = SM_OK;
    SM_init();
    err = SM_set_states(states_config, 0);
    TEST_ASSERT_TRUE(err != SM_OK);
}

TEST_CASE("State machine init correct", "[sm]") {
    SM_Response err = SM_OK;
    SM_init();
    err = SM_set_states(states_config, sizeof(states_config) / sizeof(state_config_t));
    TEST_ASSERT_TRUE(err == SM_OK);
}


TEST_CASE("State machine check change to second state", "[sm]") {
    SM_Response err = SM_OK;
    SM_init();
    err |= SM_set_states(states_config, sizeof(states_config) / sizeof(state_config_t));
    err |= SM_run();
    err |= SM_change_state(ASCENT);
    TEST_ASSERT_TRUE(err == SM_OK);
    TEST_ASSERT_TRUE(test_sm.flight_callback == TEST_VALUE);
    TEST_ASSERT_TRUE(SM_get_current_state() == LAUNCHPAD)
}

// TEST_CASE("State machine init correct", "[sm]") {
//     SM_Response err = SM_OK;
//     SM_init();
//     err |= SM_add_state(LAUNCHPAD, launchpad_callback, NULL);
//     err |= SM_add_state(FLIGHT, flight_callback, NULL);
//     err |= SM_add_state(AIR_BRAKE_ON, air_brake_callback, NULL);
//     err |= SM_add_state(APOGEE_CONFIRMED, apogee_callback, NULL);
//     err |= SM_add_state(ON_GROUND, on_ground_callback, NULL);
//     err |= SM_run();

//     TEST_ASSERT_TRUE(err == SM_OK);
// }

// TEST_CASE("Change first state", "[sm]") {
//     SM_Response err = SM_OK;
//     init_all();
//     err |= SM_change_state(FLIGHT);
//     TEST_ASSERT_TRUE(err == SM_OK);
// }

// TEST_CASE("State machine change to invalid state", "[sm]") {
//     SM_Response err = SM_OK;
//     init_all();
//     err |= SM_change_state(FLIGHT);
//     TEST_ASSERT_TRUE(err == SM_OK);

//     err |= SM_change_state(ON_GROUND);
//     TEST_ASSERT_TRUE(err != SM_OK);
// }


// TEST_CASE("State machine run all states", "[sm]") {
//     SM_Response err = SM_OK;
//     init_all();
//     err |= SM_change_state(FLIGHT);
//     err |= SM_change_state(AIR_BRAKE_ON);
//     err |= SM_change_state(APOGEE_CONFIRMED);
//     err |= SM_change_state(ON_GROUND);

//     TEST_ASSERT_TRUE(err == SM_OK);
// }

// TEST_CASE("State machine check end loop", "[sm]") {
//     init_all();
//     SM_set_end_function(end_test, 1000);
//     SM_change_state(FLIGHT);
//     SM_change_state(AIR_BRAKE_ON);
//     SM_change_state(APOGEE_CONFIRMED);
//     SM_change_state(ON_GROUND);
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     TEST_ASSERT_EQUAL(TEST_VALUE, test_sm.end_test);
// }

// TEST_CASE("State machine check callbacks", "[sm]") {
//     init_all();
//     SM_change_state(FLIGHT);
//     SM_change_state(AIR_BRAKE_ON);
//     SM_change_state(APOGEE_CONFIRMED);
//     SM_change_state(ON_GROUND);
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     TEST_ASSERT_EQUAL(TEST_VALUE, test_sm.launchpad_callback);
//     TEST_ASSERT_EQUAL(TEST_VALUE, test_sm.flight_callback);
//     TEST_ASSERT_EQUAL(TEST_VALUE, test_sm.air_brake_callback);
//     TEST_ASSERT_EQUAL(TEST_VALUE, test_sm.apogee_callback);
//     TEST_ASSERT_EQUAL(TEST_VALUE, test_sm.ground_callback);
// }


// TEST_CASE("State machine change state and get state", "[sm]") {
//     init_all();
//     SM_change_state(FLIGHT);
//     SM_change_state(AIR_BRAKE_ON);

//     TEST_ASSERT_EQUAL(AIR_BRAKE_ON, SM_get_current_state());
// }
