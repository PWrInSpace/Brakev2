// Copyright 2022 PWrInSpace

#include "unity.h"
#include "sensor.h"
#include "voltageMeasure.h"

TEST_CASE("ADC Test", "[ADC]") {
    TEST_ASSERT_EQUAL(32, sensor_return_value());
}
