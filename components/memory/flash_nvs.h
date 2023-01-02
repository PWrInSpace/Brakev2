// Copyright 2022 PWr in Space, Krzysztof Gliwiński
#pragma once

typedef enum {
  NVS_OK,
  NVS_INIT_ERROR,
  NVS_OPEN_ERROR,
  NVS_READ_ERROR,
} NVSResult;

NVSResult NVS_init(void);
NVSResult NVS_write_uint8(const char* key, uint8_t val);
NVSResult NVS_read_uint8(const char* key, uint8_t* val);
