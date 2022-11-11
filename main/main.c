// Copyright 2022 PWrInSpace

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/adc.h"
#include "voltageMeasure.h"

#define TAG "MAIN"

void app_main(void)
{
    int i = 0;
    voltageMeasure vBat = {.channel = ADC1_CHANNEL_4,
                           .adcRaw = 0,
                           .voltage = 0,
                           .voltageDivider = 3.319727891};
    voltageMeasureInit(&vBat);
    while (1)
    {
        
        ESP_LOGI(TAG, "Battery voltage: %f\n", getVoltage(&vBat));
        i++;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
