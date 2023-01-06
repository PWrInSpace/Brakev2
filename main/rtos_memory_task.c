#include "rtos_tasks.h"

#define TAG "MEM_TASK"

void memory_task(void *arg) {

    while(1) {
        // ESP_LOGI(TAG, "Hello MEMORY TASK");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}