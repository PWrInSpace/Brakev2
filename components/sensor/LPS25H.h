// Copyright 2022 PWrInSpace

#pragma once
#include <stdbool.h>
#include <stdint.h>

#include "driver/i2c.h"

typedef enum LPS25H_REG { WHO_AM_I = 0x0F; 
};

typedef struct {
  i2c_config_t conf;
  i2c_port_t port;
  uint8_t addr;
} LPS25H;

void LPS25HInit(LPS25H *lps, uint8_t sda, uint8_t scl, i2c_port_t portNum);