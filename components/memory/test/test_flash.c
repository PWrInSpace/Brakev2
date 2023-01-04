// Copyright 2022 PWr in Space, Krzysztof Gliwiński

#include <string.h>

#include "esp_log.h"
#include "flash.h"
#include "unity.h"

#define FILE_NAME (const char*)"/spiffs/test_data.txt"
#define EXPECTED_DATA \
  (const char*)"Test text, nothing to find here...\nWidzisz mie?\0"

#define TAG "Flash test"

TEST_CASE("Flash init and read file test", "[flash]") {
  FLASH_init(1);
  char readData[40];
  FlashResult res;
  res = FLASH_read_all_data(FILE_NAME, readData, 1000000);
  TEST_ASSERT_EQUAL(FLASH_OK, res);
  ESP_LOGI(TAG, "%s", readData);

  TEST_ASSERT_EQUAL(0, strcmp(EXPECTED_DATA, readData));
}
