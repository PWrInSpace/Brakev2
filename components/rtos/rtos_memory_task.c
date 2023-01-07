// Copyright 2022 PWrInSpace, Kuba
#include <string.h>
#include <memory.h>
#include "sdcard.h"
#include "rtos_tasks.h"

#define TAG "MEM_TASK"
#define FILE_NAME "/sdcard/data"
extern sd_card_t sd_card;

static bool create_path_to_file(char *file_path, size_t size) {
    char *path = (char*)calloc(size, sizeof(char));

    for (int i = 0; i < 100; ++i) {
        snprintf(path, size, "%s%d.txt", file_path, i);
        if (SD_file_exists(path) == false) {
            memcpy(file_path, path, size);
            free(path);
            return true;
        }
    }

    free(path);
    return false;
}

static bool can_save_data_to_sd(DATA_SAVE_OPTIONS option) {
    if (option == SAVE_TO_BOTH || option == SAVE_TO_SD) {
        return true;
    }

    return false;
}

static bool can_save_data_to_flash(DATA_SAVE_OPTIONS option) {
    if (option == SAVE_TO_BOTH || option == SAVE_TO_FLASH) {
        return true;
    }

    return false;
}

void memory_task(void *arg) {
    char data_string[512];
    data_to_memory_task_t data_to_save;
    char file_path[sizeof(FILE_NAME) + 6] = FILE_NAME;

    if (create_path_to_file(file_path, sizeof(file_path)) == false) {
        ESP_LOGW(TAG, "CAN'T CREATE FILE PATH, USING -> %s", file_path);
    }
    ESP_LOGI(TAG, "Using file path %s", file_path);


    while (1) {
        if (xQueueReceive(rtos.data_to_memory, (void*) &data_to_save, portMAX_DELAY) == pdTRUE) {
            if (can_save_data_to_sd(data_to_save.save_option) == true) {
                snprintf(data_string, sizeof(data_string), "DUPA;%d;%ld",
                        data_to_save.data.state, data_to_save.data.up_time);
                ESP_LOGI(TAG, "SAVING TO SD: %s", data_string);
                SD_write(&sd_card, file_path, data_string, sizeof(data_string));
            }

            if (can_save_data_to_flash(data_to_save.save_option) == true) {
                ESP_LOGD(TAG, "SAVING TO FLASH");
            }
        }
    }
}
