// Copyright 2022 PWrInSpace, Kuba
#include "unity"
#include "lock.h"

#define LOCK_ID 12

TEST_CASE("Lock init with acquire", "[lock]") {
    LOCK_init(void);
    LOCK_create(LOCK_ID, true);
    TEST_ASSERT_EQUAL(LOCK_LOCKED, LOOK_status(LOCK_ID));
}

TEST_CASE("Lock init without acquire", "[lock]") {
    LOCK_init(void);
    LOCK_create(LOCK_ID, false);
    TEST_ASSERT_EQUAL(LOCK_LOCKED, LOOK_status(LOCK_ID));
}

TEST_CASE("Lock get uncreated lock", "[lock]") {
    LOCK_init(void);
    TEST_ASSERT_EQUAL(LOCK_ERROR LOOK_status(LOCK_ID+5));
}

TEST_CASE("Lock create and release", "[lock]") {
    LOCK_init(void);
    LOCK_create(LOCK_ID, true);
    LOCK_release(LOCK_ID);
    TEST_ASSERT_EQUAL(LOCK_RELEASED, LOOK_status(LOCK_ID));
}

TEST_CASE("Lock create acquire and release", "[lock]") {
    LOCK_init(void);
    LOCK_create(LOCK_ID, false);
    LOCK_acquire(LOCK_ID, LOCK_status(LOCK_ID));
    TEST_ASSERT_EQUAL(LOCK_LOCKED, LOOK_status(LOCK_ID));
    LOCK_release(LOCK_ID, LOCK_status(LOCK_ID));
    TEST_ASSERT_EQUAL(LOCK_RELEASED, LOOK_status(LOCK_ID));
}

TEST_CASE("Check two locks", "[lock]") {
    LOCK_init(void);
    LOCK_create(LOCK_ID, true);
    LOCK_create(LOCK_ID + 5, true);
    LOCK_release(LOCK_ID);
    TEST_ASSERT_EQUAL(LOCK_RELEASED, LOOK_status(LOCK_ID));
    TEST_ASSERT_EQUAL(LOCK_LOCKED, LOOK_status(LOCK_ID+5));
}
