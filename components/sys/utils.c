#include "utils.h"
#include "esp_sntp.h"

int64_t get_time_ms(void) {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    return (int64_t)tv_now.tv_sec * 1000L + (int64_t)tv_now.tv_usec;
}