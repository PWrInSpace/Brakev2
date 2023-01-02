// Copyright 2022 PWr in Space, Krzysztof Gliwiński
#pragma once

#include <stddef.h>
#include <stdint.h>

#include <esp_flash.h>
#include <esp_flash_spi_init.h>
#include <esp_spi_flash.h>
#include <string.h>

#include "esp_log.h"
#include "esp_spiffs.h"

// flash memory to storage huge data files
// usage:
// you have to create spiffs partition in flash memory:
// 1. Create csv file in project dir, <name>.txt
// 2. add flag to platformio.ini file
// board_build.partitions = <name>.csv
// 3. run program and check logs and assertions,
// you'll probably have to format created partition
// using FLASH_format()
//
// if you are using partition table given above, path should start with /spiffs/
// example: /spiffs/test.txt
//
// example of partiotion file:
// nvs,      data, nvs,     0x9000,  0x6000,
// phy_init, data, phy,     0xf000,  0x1000,
// factory,  app,  factory, 0x10000, 1M,
// storage,  data, spiffs,  ,        0xF0000,
//
// be aware that flash memory has a finite number of erase-write cycles

typedef enum {
  FLASH_OK,
  FLASH_INIT_ERROR,
  FLASH_ALREADY_INITIALIZED,
  FLASH_IS_NOT_INITIALIZED,
  FLASH_CHECK_FAILED,
  FLASH_WRITE_ERROR,
  FLASH_OPEN_ERROR,
  FLASH_FORMAT_ERROR,
  FLASH_READ_NO_DATA,
  FLASH_READ_ALL_SIZE,
  FLASH_ALLOC_ERROR,
} FlashResult;

#define PATH "/spiffs"
#define MAX_FILES 1

FlashResult FLASH_init(uint8_t max_files);
FlashResult FLASH_write(const char* file_name, const char* data, size_t size);
FlashResult FLASH_read_all_data(const char* file_name, char* data_container,
                                size_t size);
size_t FLASH_get_used_size(void);
FlashResult FLASH_format(void);

