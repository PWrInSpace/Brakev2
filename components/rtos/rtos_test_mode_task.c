// Copyright 2022 PWrInSpace, Kuba
#include "rtos_tasks.h"

#define TAG "TEST"

void test_mode_task(void *arg) {
    char buffer[1000] ={0};
    int length = 0;
    UART_flush_rx(&uart);
    UART_flush_tx(&uart);

    while (1) {
        // read from uart
        length = UART_read_until(&uart, buffer, sizeof(buffer), '\r', pdMS_TO_TICKS(500));
        if (length > 0) {
            ESP_LOGE(TAG, "Read from serial\nlength: %d\ntext: %s", length, buffer);
        }
        // calculate data
        // send as new data event
        // restart on end command
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
