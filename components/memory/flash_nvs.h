// Copyright 2022 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stdint.h>

#include "nvs_flash.h"
#include "esp_log.h"

typedef enum {
  NVS_OK,
  NVS_INIT_ERROR,
  NVS_OPEN_ERROR,
  NVS_READ_ERROR,
} NVSResult;

NVSResult NVS_init(void);
NVSResult NVS_write_uint8(const char* key, uint8_t val);
NVSResult NVS_read_uint8(const char* key, uint8_t* val);
