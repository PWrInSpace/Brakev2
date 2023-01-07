// Copyright 2022 PWr in Space, Krzysztof Gliwiński

#include <string.h>

#include "esp_log.h"
#include "flash.h"
#include "unity.h"

#define READ_FILE_NAME (const char*)"/spiffs/test_read_data.txt"
#define WRITE_FILE_NAME (const char*)"/spiffs/test_write_data.txt"
#define EXPECTED_DATA \
  (const char*)"Test text, nothing to find here...\nWidzisz mie?\0"
#define WRITE_DATA (const char*)"Lorem ipsum\0"

#define TAG "Flash test"

TEST_CASE("Flash init and read file test", "[flash]") {
  TEST_ASSERT_EQUAL(FLASH_OK, FLASH_init(1));
}

TEST_CASE("Flash read file test", "[flash") {
  char readData[40];
  FlashResult res;
  res = FLASH_read_all_data(READ_FILE_NAME, readData, 1000000);
  TEST_ASSERT_EQUAL(FLASH_OK, res);
  // ESP_LOGI(TAG, "%s", readData);

  TEST_ASSERT_EQUAL_STRING(EXPECTED_DATA, readData);
}

TEST_CASE("Flash write file test", "[flash") {
  char readData[40];
  FlashResult res;
  res = FLASH_write(WRITE_FILE_NAME, WRITE_DATA, sizeof(WRITE_DATA));
  TEST_ASSERT_EQUAL(FLASH_OK, res);
  res = FLASH_read_all_data(WRITE_FILE_NAME, readData, 1000000);
  TEST_ASSERT_EQUAL(FLASH_OK, res);
  // ESP_LOGI(TAG, "%s", readData);

  TEST_ASSERT_EQUAL_STRING(WRITE_DATA, readData);
}
