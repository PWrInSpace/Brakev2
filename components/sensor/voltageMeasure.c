// Copyright 2022 PWrInSpace

#include "voltageMeasure.h"

bool voltageMeasureInit(voltageMeasure *vMes)
{
    // vMes->channel = channel;
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(vMes->channel, ADC_ATTEN_DB_11);

    return true;
}

float getVoltage(voltageMeasure *vMes)
{
    vMes->adcRaw = adc1_get_raw(vMes->channel);
    vMes->voltage = (float)vMes->adcRaw * vMes->voltageDivider * 3.3f / 4095.0f;
    return vMes->voltage;
}

uint16_t getRawVoltage(voltageMeasure *vMes)
{
    vMes->adcRaw = adc1_get_raw(vMes->channel);
    vMes->voltage = (float)vMes->adcRaw * vMes->voltageDivider * 3.3f / 4095.0f;
    return vMes->adcRaw;
}