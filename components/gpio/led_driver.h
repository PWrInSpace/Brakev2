// Copyright 2023 PWr in Space,
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/ledc.h"
#include "esp_log.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT  // Set duty resolution to 13 bits
#define LEDC_FREQUENCY (5000)  // Frequency in Hertz. Set frequency at 5 kHz
#define MAX_LED_DUTY 8190

#define LED_DRIVER_TAG "LED Driver"

/*!
    \brief Intended for LEDs that are positive voltage driven!
*/
typedef struct {
  uint8_t led_gpio_num;
  uint8_t ledc_channel_num;
  uint8_t ledc_timer_num;
} led_driver_t;

bool led_driver_init(led_driver_t *led_drv, uint8_t led_gpio_num,
                     uint8_t ledc_channel_num);

bool led_driver_update_duty_cycle(led_driver_t *led_drv,  uint16_t duty);