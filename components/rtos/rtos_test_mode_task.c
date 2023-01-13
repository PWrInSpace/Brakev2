// Copyright 2022 PWrInSpace, Kuba
#include "rtos_tasks.h"
#include "esp_system.h"
#include "LSM6DS3.h"

#define TAG "TEST"

#define EXIT_COMMAND "exit\r"

bool check_if_exit_command(char* data) {
    if (strcmp(data, EXIT_COMMAND) == 0) {
        return true;
    }

    return false;
}

void test_mode_task(void *arg) {
    char buffer[1000] ={0};
    int length = 0;
    LSM6DS3_acc_t acc = {0};
    LSM6DS3_gyro_t gyro = {0};
    float altittude = 0;
    float pressure = 0;
    UART_flush_rx(&uart);
    UART_flush_tx(&uart);

    while (1) {
        // read from uart
        length = UART_read_until(&uart, buffer, sizeof(buffer), '\r', pdMS_TO_TICKS(500));
        if (length > 0) {
            ESP_LOGE(TAG, "Read from serial\nlength: %d\ntext: %s", length, buffer);
            if (check_if_exit_command(buffer) == true) {
                esp_restart();
            }

            if (sscanf(buffer, "%f;%f;%f;%f;%f;%f;%f;%f", &acc.x, &acc.y, &acc.z,
                       &gyro.x, &gyro.y, &gyro.z, &pressure, &altittude) < 8) {
                ESP_LOGE(TAG, "READ ERRORR");
            }
        }
        // calculate data
        // send as new data event
        // restart on end command
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
