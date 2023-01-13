// Copyright 2022 PWrInSpace

#include "console_commands.h"
#include "state_machine.h"
#include "esp_log.h"
#include "esp_system.h"
#include "flash_nvs.h"
#include "rtos_tasks.h"

#define TAG "CLI"

int CLI_state_machine_get_state(int argc, char **argv) {
    uint8_t state = SM_get_current_state();
    ESP_LOGI(TAG, "Current state %d", state);
    return 0;
}

int CLI_reset_device(int argc, char **argv) {
    esp_restart();
    return 0;
}


int CLI_reset_reason(int argc, char **argv) {
    esp_reset_reason_t reason = esp_reset_reason();
    ESP_LOGI(TAG, "Reset reason %d", reason);
    return 0;
}

int CLI_turn_on_test_mode(int argc, char **argv) {
    if (NVS_write_uint8(NVS_TEST_MODE, TEST_MODE_ON) == NVS_OK) {
        ESP_LOGI(TAG, "TEST MODE ON");
        esp_restart();
        return 0;
    }

    ESP_LOGE(TAG, "Unable to write to nvs storage, test mode aborted");

    return 1;
}