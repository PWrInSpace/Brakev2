// Copyright 2022 PWrInSpace

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "console.h"
#include "console_commands.h"
#include "state_machine.h"

#define TAG "MAIN"

esp_console_cmd_t console_commands[] = {
    {"test", "test1234", NULL, CLI_test, NULL},
    {"sm-state", "Get current state", NULL, CLI_state_machine_get_state, NULL},
};


void app_main(void) {
    int i = 0;
    console_init();
    console_register_commands(console_commands,
        sizeof(console_commands)/sizeof(console_commands[0]));
    SM_init();

    while (1) {
        ESP_LOGI(TAG, "[%d] Hello world!\n", i);
        i++;
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
