// Copyright 2022 PWrInSpace

#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "driver/adc.h"
#include "soc/adc_channel.h"
// #include "esp_adc_cal.h"

typedef struct {
  adc1_channel_t channel;
  uint16_t adcRaw;
  float voltage;
  float voltageDivider;
} voltageMeasure;

bool voltageMeasureInit(voltageMeasure *vMes);

float getVoltage(voltageMeasure *vMes);

uint16_t getRawVoltage(voltageMeasure *vMes);
