#ifndef FLIGHT_TIME_H
#define FLIGHT_TIME_H

#include <stdint.h>

void FLIGHT_TIME_start(int64_t start_time);
int64_t FLIGHT_TIME_get_time(int64_t time_now);

#endif