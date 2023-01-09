// Copyright 2022 PWrInSpace, Kuba
#ifndef I2C_SIMPLE
#define I2C_SIMPLE

#include "driver/i2c.h"

typedef i2c_config_t i2c_t;

bool I2C_init(i2c_t *i2c, i2c_port_t port, uint8_t sda, uint8_t scl);

#endif
