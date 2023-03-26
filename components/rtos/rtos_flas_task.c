// Copyright 2022 PWrInSpace, Kuba
#include <string.h>
#include <memory.h>
#include "sdcard.h"
#include "rtos_tasks.h"
#include "utils.h"
#include "flash.h"

#define TAG "MEM_TASK"

static void open_flash_file(FILE **flash_file, bool* flash_first_write) {
    if (*flash_file != NULL) {
        return;
    }
    // if (*flash_first_write == true) {
    //     *flash_first_write = false;
    //     *flash_file = fopen("/spiffs/data.txt", "w");
    //     fclose(*flash_file);
    // }

    *flash_file = fopen("/spiffs/data.txt", "a");
}

static void save_data_to_flash(FILE *flash_file, rocket_data_t* data, size_t size) {
    if (FLASH_is_memory_available() == false) {
        ESP_LOGE(TAG, "FLASH FULL");
        return;
    }

    if (flash_file == NULL) {
        ESP_LOGE(TAG, "FLASH FILE NULL");
        return;
    }

    fwrite(data, size, 1, flash_file);
}

static void close_flash_file(FILE *flash_file) {
    if (flash_file != NULL) {
        return;
    }

    fclose(flash_file);
    flash_file = NULL;
}

void flash_task(void *arg) {
    rocket_data_t data;

    bool flash_first_write = true;
    FILE *flash_file = NULL;
    uint32_t start = 0;
    uint32_t stop = 0;

    // waiting for signal
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    flash_file = fopen("/spiffs/data.txt", "w");
    close_flash_file(flash_file);

    while (1) {
        if (uxQueueMessagesWaiting(rtos.data_to_flash) > RTOS_FLASH_SAVE) {
            start = get_time_ms();
            open_flash_file(&flash_file, &flash_first_write);
            while (uxQueueMessagesWaiting(rtos.data_to_flash) > 0) {
                if (xQueueReceive(rtos.data_to_flash, (void*) &data, portMAX_DELAY) == pdTRUE) {
                    save_data_to_flash(flash_file, &data, sizeof(data));
                }
            }
            close_flash_file(flash_file);
            stop = get_time_ms();
            ESP_LOGI(TAG, "FLASH DURATION %d", stop-start);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
