#include "flight_time.h"

static int64_t gb_start_time = 0;

void FLIGHT_TIME_start(int64_t start_time) {
    gb_start_time = start_time;
}


int64_t FLIGHT_TIME_get_time(int64_t time_now) {
    if (gb_start_time == 0) {
        return 0;
    }
    return time_now - gb_start_time;
}