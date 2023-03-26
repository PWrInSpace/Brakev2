// Copyright 2022 PWrInSpace, Kuba
#include <string.h>
#include <memory.h>
#include "sdcard.h"
#include "rtos_tasks.h"
#include "utils.h"
#include "flash.h"

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

size_t create_data_csv(data_to_memory_task_t *rec, char *data_string, size_t len) {
    return snprintf(data_string, len,
        "%lu;%d;%lu;%.2f;%.2f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f\n",
        (uint32_t)rec->data.flight_time, rec->data.state, (uint32_t)rec->data.up_time,
        rec->data.sensors.filtered.height, rec->data.sensors.velocity,
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


static void save_data_to_sd(data_to_memory_task_t *data, char *data_buffer,
                            size_t size, char* file_path) {
    if (can_save_data_to_sd(data->save_option) == false) {
        return;
    }

    create_data_csv(data, data_buffer, size);
    SD_write(&sd_card, file_path, data_buffer);
    if (SETI_get_settings()->test_mode == true) {
        ESP_LOGI(TAG, ";%s", data_buffer);
    }
}

static void open_flash_file(FILE **flash_file, bool* flash_first_write) {
    if (*flash_file != NULL) {
        return;
    }
    if (*flash_first_write == true) {
        *flash_first_write = false;
        *flash_file = fopen("/spiffs/data.txt", "w");
        fclose(flash_file);
    }

    flash_file = fopen("/spiffs/data.txt", "a");
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

void memory_task(void *arg) {
    char data_string[512];
    data_to_memory_task_t data;
    char file_path[sizeof(FILE_NAME) + 6] = FILE_NAME;

    if (create_path_to_file(file_path, sizeof(file_path)) == false) {
        ESP_LOGW(TAG, "CAN'T CREATE FILE PATH, USING -> %s", file_path);
    }
    ESP_LOGI(TAG, "Using file path %s", file_path);

    SD_write(&sd_card, file_path, file_header);
    bool flash_first_write = true;
    FILE *flash_file = NULL;
    uint32_t start = 0;
    uint32_t stop = 0;
    while (1) {
        if (uxQueueMessagesWaiting(rtos.data_to_memory) > 10) {
            start = get_time_ms();
            while (uxQueueMessagesWaiting(rtos.data_to_memory) > 0) {
                if (xQueueReceive(rtos.data_to_memory, (void*) &data, portMAX_DELAY) == pdTRUE) {
                    save_data_to_sd(&data, data_string, sizeof(data_string), file_path);

                    if (can_save_data_to_flash(data.save_option) == true) {
                        open_flash_file(&flash_file, &flash_first_write);
                        save_data_to_flash(flash_file, &data.data, sizeof(data.data));
                    }
                }
            }
            close_flash_file(flash_file);
            stop = get_time_ms();
            ESP_LOGI(TAG, "DURATION %d", stop-start);
        }
    }
}
