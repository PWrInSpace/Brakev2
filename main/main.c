// Copyright 2022 PWrInSpace

#include <stdio.h>

#include "esp_log.h"
#include "console.h"
#include "console_commands.h"
#include "state_machine.h"
#include "LPS25H.h"
#include "LSM6DS3.h"
#include "sdcard.h"
#include "config.h"
#include "rtos_tasks.h"
#include "brejk_system.h"

#define TAG "MAIN"

esp_console_cmd_t console_commands[] = {
    {"test", "test1234", NULL, CLI_test, NULL},
    {"sm-state", "Get current state", NULL, CLI_state_machine_get_state, NULL},
    {"reset-dev", "Software reset", NULL, CLI_reset_device, NULL},
    {"reset-reason", "Get reset reason", NULL, CLI_reset_reason, NULL},
};

void app_main(void) {
    spi_init();
    i2c_sensor_init();
    SM_init();
    console_init();
    console_register_commands(console_commands,
        sizeof(console_commands)/sizeof(console_commands[0]));

    event_loop_init();
    event_loop_register();
    rtos_init();

    // rtos_test_mode_init();


    vTaskDelete(NULL);
}
