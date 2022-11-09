// Copyright 2022 PWrInSpace

#include "unity.h"
#include "sensor.h"


TEST_CASE("Sensor test", "[SENSOR]") {
    TEST_ASSERT_EQUAL(32, sensor_return_value());
}
