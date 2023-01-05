// Copyright 2022 PWrInSpace

#include "console_commands.h"
#include "state_machine.h"
#include "esp_log.h"

#define TAG "CLI"

int CLI_test(int argc, char** argv) {
    ESP_LOGI(TAG, "Hello test");
    return 0;
}

int CLI_state_machine_get_state(int argc, char **argv) {
    uint8_t state = SM_get_current_state();
    ESP_LOGI(TAG, "Current state %d", state);
    return 0;
}
