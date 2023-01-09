// Copyright 2022 PWrInSpace

#include "esp_log.h"
#include "sensor.h"
#include "unity.h"
#include "voltageMeasure.h"

#define BATT_ADC_CHANNEL ADC_CHANNEL_4
#define BATT_ADC_CAL 3.3f
#define VMES_LOG_TAG "vMes_test"
static VoltageMeasure vMes;

TEST_CASE("VoltageMeasure init test", "[ADC]") {
  TEST_ASSERT_EQUAL(adcOK,
                    voltageMeasureInit(&vMes, BATT_ADC_CHANNEL, BATT_ADC_CAL));
}

TEST_CASE("VoltageMeasure read test raw (7.4V on BATT ADC)", "[ADC]") {
  // expected around 2850 in raw, 2,31V in voltage
  int vRaw = voltageMeasureReadRaw(&vMes);
  ESP_LOGI(VMES_LOG_TAG, "%d", vRaw);
  TEST_ASSERT_INT_WITHIN(30, 2800, vRaw);
}

TEST_CASE("VoltageMeasure read test voltage (7.4V on BATT ADC)", "[ADC]") {
  float vTest = voltageMeasureReadVoltage(&vMes);
  ESP_LOGI(VMES_LOG_TAG, "%f", vTest);
  TEST_ASSERT_FLOAT_WITHIN(0.2f, 7.4f, vTest);
}
