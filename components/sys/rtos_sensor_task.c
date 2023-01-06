#include "rtos_tasks.h"

#define TAG "SENSOR TASK"

void sensor_task(void *arg) {

    while (1) {
        // ESP_LOGI(TAG, "Hello SENSOR_TASK");
        esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_NEW_DATA_EVENT, NULL, 0, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}