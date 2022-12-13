// Copyright 2022 PWrInSpace

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "voltageMeasure.h"
#include "console.h"

#define TAG "MAIN"


int test(int argc, char** argv) {
    ESP_LOGI(TAG, "Hello test");
    return 0;
}

esp_console_cmd_t console_commands[] = {
    {"test", "test1234", NULL, test, NULL}
};


void app_main(void) {
    int i = 0;
    console_init();
    console_register_commands(console_commands, sizeof(console_commands)/sizeof(console_commands[0]));



    while (1) {
        ESP_LOGI(TAG, "[%d] Hello world!\n", i);
        i++;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
