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

static size_t create_data_csv(data_to_memory_task_t *rec, char *data_string, size_t len) {
    return snprintf(data_string, len,
        "%lu;%d;%lu;%.2f;%.2f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n",
        (uint32_t)rec->data.flight_time, rec->data.state, (uint32_t)rec->data.up_time, rec->data.sensors.filtered.height,
        rec->data.sensors.velocity,
        rec->data.sensors.acc.x, rec->data.sensors.acc.y, rec->data.sensors.acc.z,
        rec->data.sensors.gyro.x, rec->data.sensors.gyro.y, rec->data.sensors.gyro.z,
        rec->data.sensors.height,
        rec->data.sensors.pressure, rec->data.sensors.temp,
        rec->data.sensors.filtered.acc.x, rec->data.sensors.filtered.acc.y,
        rec->data.sensors.filtered.acc.z,
        rec->data.sensors.filtered.gyro.x, rec->data.sensors.filtered.gyro.y,
        rec->data.sensors.filtered.gyro.z,
        rec->data.sensors.vBatt);
}

const char file_header[] = "FLIGHT TIME; STATE; UPTIME; F ALTITUDE; VELOCITY; ACC X; ACC Y; ACC Z;"
                    "GYRO X; GYRO; Y; GYRO Z; ALTITUDE; PRESSURE; TEMPERATURE; F ACC X; F ACC Y;"
                    "F ACC Z F GYRO X; F GYRO Y; F GYRO Z; VBATT\n";

void memory_task(void *arg) {
    char data_string[512];
    data_to_memory_task_t data;
    char file_path[sizeof(FILE_NAME) + 6] = FILE_NAME;

    if (create_path_to_file(file_path, sizeof(file_path)) == false) {
        ESP_LOGW(TAG, "CAN'T CREATE FILE PATH, USING -> %s", file_path);
    }
    ESP_LOGI(TAG, "Using file path %s", file_path);

    SD_write(&sd_card, file_path, file_header, sizeof(file_header));

    while (1) {
        if (xQueueReceive(rtos.data_to_memory, (void*) &data, portMAX_DELAY) == pdTRUE) {
            if (can_save_data_to_sd(data.save_option) == true) {
                create_data_csv(&data, data_string, sizeof(data_string));
                SD_write(&sd_card, file_path, data_string, sizeof(data_string));

                if (SETI_get_settings()->test_mode == true) {
                    ESP_LOGI(TAG, ";%s", data_string);
                }
            }

            if (can_save_data_to_flash(data.save_option) == true) {
                ESP_LOGD(TAG, "SAVING TO FLASH");
            }
        }
    }
}
