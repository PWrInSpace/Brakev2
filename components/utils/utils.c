// Copyright 2022 PWrInSpace, Kuba

#include "utils.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

uint64_t get_time_ms(void) {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);

    return (uint64_t) (esp_timer_get_time() / 1000ULL);
}
