// Copyright 2022 PWrInSpace

#include <stdlib.h>
#include "console_commands.h"
#include "state_machine.h"
#include "esp_log.h"
#include "esp_system.h"
#include "brake_servo.h"
#include "recovery_servo.h"
#include "flash_nvs.h"
#include "rtos_tasks.h"
#include "nvs.h"

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

int CLI_brake_move(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    int angle = atoi(argv[1]);
    ESP_LOGI(TAG, "Brake servo -> angle %d", angle);
    if (BRAKE_SERVO_move(angle) == false) {
        ESP_LOGE(TAG, "Unable to move servo");
        return 1;
    }

    return 0;
}

int CLI_recov_move(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    int angle = atoi(argv[1]);
    if (RECOV_SERVO_move(angle) == false) {
        ESP_LOGE(TAG, "Unable to move servo");
        return 1;
    }

    ESP_LOGI(TAG, "Recovery servo -> angle %d", angle);
    return 0;
}

int CLI_change_state(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    int state = atoi(argv[1]);
    switch (state) {
        case ASCENT:
            esp_event_post_to(event_get_handle(), TASK_EVENTS, SENSORS_HIGH_ACC_EVENT,
                      NULL, 0, portMAX_DELAY);
            break;
        default:
            if (SM_change_state(state) != SM_OK) {
                ESP_LOGE(TAG, "Unable to change state");
                return 1;
            }
    }
    return 0;
}

int CLI_set_brake_open_angle(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t angle = atoi(argv[1]);
    if (angle > BRAKE_SERVO_MAX_DEGREE) {
        ESP_LOGW(TAG, "Invalid angle");
        return 1;
    }
    if (NVS_write_uint16(NVS_BRAKE_OPEN_ANGLE, angle) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->brake_open_angle = angle;
    ESP_LOGI(TAG, "New angle %d", angle);
    return 0;
}


int CLI_set_brake_close_angle(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t angle = atoi(argv[1]);
    if (angle > BRAKE_SERVO_MAX_DEGREE) {
        ESP_LOGW(TAG, "Invalid angle");
        return 1;
    }

    if (NVS_write_uint16(NVS_BRAKE_CLOSE_ANGLE, angle) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->brake_close_angle = angle;
    ESP_LOGI(TAG, "New angle %d", angle);
    return 0;
}

int CLI_set_recovery_open_angle(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t angle = atoi(argv[1]);
    if (angle > RECOV_SERVO_MAX_DEGREE) {
        ESP_LOGW(TAG, "Invalid angle");
        return 1;
    }
    if (NVS_write_uint16(NVS_RECOVERY_OPEN_ANGLE, angle) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->recovery_open_angle = angle;
    ESP_LOGI(TAG, "New angle %d", angle);
    return 0;
}


int CLI_set_recovery_close_angle(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t angle = atoi(argv[1]);
    if (angle > RECOV_SERVO_MAX_DEGREE) {
        ESP_LOGW(TAG, "Invalid angle");
        return 1;
    }
    if (NVS_write_uint16(NVS_RECOVERY_CLOSE_ANGLE, angle) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->recovery_close_angle = angle;
    ESP_LOGI(TAG, "New angle %d", angle);
    return 0;
}

int CLI_set_alpha(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    float alpha = atof(argv[1]);
    if (NVS_write_uint8(NVS_ALPHA, alpha * 100) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->alpha = alpha;
    ESP_LOGI(TAG, "New alpha %f", alpha);
    return 0;
}

int CLI_set_beta(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    float beta = atof(argv[1]);
    if (NVS_write_uint8(NVS_BETA, beta * 100) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->beta = beta;
    ESP_LOGI(TAG, "New alpha %f", beta);
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

int CLI_buzzer_active(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint8_t active = atoi(argv[1]) > 0 ? 1 : 0;
    if (NVS_write_uint8(NVS_BUZZER_ACTIVE, active) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->buzzer_active = active;
    ESP_LOGI(TAG, "Buzzer active: %d", active);
    return 0;
}

int CLI_set_brake_open_time(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t time = atoi(argv[1]);
    if (NVS_write_uint16(NVS_BRAKE_OPEN_TIME, time) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->brake_open_time = time;
    ESP_LOGI(TAG, "New time %d", time);
    return 0;
}

int CLI_set_safety_trigger_time(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t time = atoi(argv[1]);
    if (NVS_write_uint16(NVS_RECOV_SAFETY_TRIG_TIME, time) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->recovery_safety_trigger = time;
    ESP_LOGI(TAG, "New time %d", time);
    return 0;
}

int CLI_set_recovery_open_time(int argc, char **argv) {
    if (argc != 2) {
        ESP_LOGW(TAG, "Invalid number of arguments");
        return 1;
    }

    uint16_t time = atoi(argv[1]);
    if (NVS_write_uint16(NVS_RECOV_OPEN_TIME, time) != NVS_OK) {
        ESP_LOGE(TAG, "Unable to write data");
        return 1;
    }

    SETI_get_settings()->recovery_open_time = time;
    ESP_LOGI(TAG, "New time %d", time);
    return 0;
}

int CLI_flash_read(int argc, char **argv) {
    esp_log_level_set("*", ESP_LOG_NONE);
    FILE *file = NULL;
    file = fopen("/spiffs/data.txt", "r");
    if (file == NULL) {
        esp_log_level_set("*", ESP_LOG_INFO);
        return -1;
    }

    data_to_memory_task_t data;
    rocket_data_t rocket_data;
    char data_buffer[200];
    while (fread(&rocket_data, sizeof(rocket_data), 1, file)) {
        data.data = rocket_data;
        create_data_csv(&data, data_buffer, sizeof(data_buffer));
        printf("%s", data_buffer);
    }
    fclose(file);

    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI(TAG, "Read end");

    return 0;
}

int CLI_print_settings(int argc, char **argv) {
    settings_t* settings = SETI_get_settings();
    ESP_LOGI(TAG, "Brake open angle: %d", settings->brake_open_angle);
    ESP_LOGI(TAG, "Brake close angle: %d", settings->brake_close_angle);
    ESP_LOGI(TAG, "Brake open time: %d", settings->brake_open_time);
    ESP_LOGI(TAG, "Recovery open angle: %d", settings->recovery_open_angle);
    ESP_LOGI(TAG, "Recovery close angle: %d", settings->recovery_close_angle);
    ESP_LOGI(TAG, "Recovery safety trigger time: %d", settings->recovery_safety_trigger);
    ESP_LOGI(TAG, "Recovery open time: %d", settings->recovery_open_time);
    // ESP_LOGI(TAG, "Alpha: %f", settings->alpha);
    // ESP_LOGI(TAG, "Beta: %f", settings->beta);
    ESP_LOGI(TAG, "Mode: %s", settings->test_mode == TEST_MODE_ON ? "Test" : "Normal");
    ESP_LOGI(TAG, "Buzzer active: %s", settings->buzzer_active == true ? "true" : "false");
    return 0;
}
