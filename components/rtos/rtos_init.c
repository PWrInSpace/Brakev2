// Copyright 2022 PWrInSpace, Kuba

#include "config.h"
#include "console.h"
#include "console_commands.h"
#include "esp_log.h"
#include "rtos_tasks.h"
#include "esp_system.h"

#define TAG "INIT"
#define WTAG "WATCHDOG"

#define BATT_ADC_CAL 3.3f

rtos_t rtos;
spi_t sd_spi;
i2c_t i2c_sensors;
uart_t uart;
sd_card_t sd_card;
LSM6DS3_t acc_sensor;
LPS25H press_sensor;
VoltageMeasure vMes;

static void wh(TaskHandle_t han) {
  if (han == rtos.sensor_task) {
    ESP_LOGE(WTAG, "Sensor task malfunction");

  } else if (han == rtos.main_task) {
    ESP_LOGE(WTAG, "Main task malfunction");
  } else if (han == rtos.memory_task) {
    ESP_LOGE(WTAG, "Memory task malfunction");
  } else if (han == rtos.test_mode_task) {
    ESP_LOGE(WTAG, "Test mode task malfunction");
  } else {
    ESP_LOGE(WTAG, "Watchdog handle raised without proper task handle");
  }
}

esp_console_cmd_t console_commands[] = {
    {"sm-state", "Get current state", NULL, CLI_state_machine_get_state, NULL},
    {"sm-change-state", "sm_change_state", NULL, CLI_change_state, NULL},
    {"reset-dev", "Software reset", NULL, CLI_reset_device, NULL},
    {"reset-reason", "Get reset reason", NULL, CLI_reset_reason, NULL},
    {"brake-servo-move", "Move brake servo", NULL, CLI_brake_move, NULL},
    {"recov-servo-move", "Move recovery servo", NULL, CLI_recov_move, NULL},
    {"print-settings", "Print dev settings", NULL, CLI_print_settings, NULL},
    {"test-mode", "Run dev in test-mode", NULL, CLI_turn_on_test_mode, NULL},
    {"brake-open", "Set brake open angle", NULL, CLI_set_brake_open_angle, NULL},
    {"brake-open-time", "Set brake open time", NULL, CLI_set_brake_open_time, NULL},
    {"brake-close", "Set brake close angle", NULL, CLI_set_brake_close_angle, NULL},
    {"recov-open", "Set recovery open angle", NULL, CLI_set_recovery_open_angle, NULL},
    {"recov-close", "Set recovery close angle", NULL, CLI_set_recovery_close_angle, NULL},
    {"recov-trigg", "Set recovery safety trigger", NULL, CLI_set_safety_trigger_time, NULL},
    {"recov-open-time", "Set recovery open time", NULL, CLI_set_recovery_open_time, NULL},
    {"alpha", "Set alpha", NULL, CLI_set_alpha, NULL},
    {"beta", "Set beta", NULL, CLI_set_beta, NULL},
    {"buzzer-active", "Buzzer active after startup", NULL, CLI_buzzer_active, NULL},
};

state_config_t states_conf[] = {
    {LAUNCHPAD, NULL, NULL},
    {ASCENT, NULL, NULL},
    {BRAKE, NULL, NULL},
    {DESCENT, NULL, NULL},
    {GROUND, NULL, NULL}
};

bool rtos_init(void) {
  rtos.data_to_memory = xQueueCreate(20, sizeof(data_to_memory_task_t));

  if (rtos.data_to_memory == NULL) {
    return false;
  }

  xTaskCreatePinnedToCore(sensor_task, "sensor_task", 8000, NULL,
                          TASK_PRIORITY_HIGH, &rtos.sensor_task, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL,
                          TASK_PRIORITY_HIGH, &rtos.main_task, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL,
                          TASK_PRIORITY_MID, &rtos.memory_task, APP_CPU_NUM);

  if (rtos.sensor_task == NULL || rtos.main_task == NULL ||
      rtos.memory_task == NULL) {
    return false;
  }

  return true;
}

bool rtos_test_mode_init(void) {
  rtos.data_to_memory = xQueueCreate(20, sizeof(data_to_memory_task_t));

  if (rtos.data_to_memory == NULL) {
    return false;
  }

  xTaskCreatePinnedToCore(test_mode_task, "sensor_task", 8000, NULL,
                          TASK_PRIORITY_HIGH, &rtos.sensor_task, APP_CPU_NUM);
  xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL,
                          TASK_PRIORITY_HIGH, &rtos.main_task, APP_CPU_NUM);
  xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL,
                          TASK_PRIORITY_MID, &rtos.memory_task, APP_CPU_NUM);

  if (rtos.sensor_task == NULL || rtos.main_task == NULL ||
      rtos.memory_task == NULL) {
    return false;
  }

  return true;
}

static bool i2c_num1_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data,
                          size_t len) {
  if (i2c_master_write_read_device(I2C_NUM_1, dev_addr, &reg_addr, 1, data, len,
                                   pdMS_TO_TICKS(100)) == ESP_OK) {
    return true;
  }

  return false;
}

static bool i2c_num1_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data,
                           size_t len) {
  uint8_t tmp[2] = {reg_addr, data[0]};
  if (i2c_master_write_to_device(I2C_NUM_1, dev_addr, tmp, sizeof(tmp),
                                 pdMS_TO_TICKS(100)) == ESP_OK) {
    return true;
  }

  return false;
}

// static void set_keys_to_flash(void) {
//   NVS_write_uint16(NVS_BRAKE_OPEN_ANGLE, 0);
//   NVS_write_uint16(NVS_BRAKE_CLOSE_ANGLE, 0);
//   NVS_write_uint16(NVS_RECOVERY_OPEN_ANGLE, 0);
//   NVS_write_uint16(NVS_RECOVERY_CLOSE_ANGLE, 0);
//   NVS_write_uint8(NVS_ALPHA, 0);
//   NVS_write_uint8(NVS_BETA, 0);
//   NVS_write_uint8(NVS_TEST_MODE, 0);
//   NVS_write_uint8(NVS_BUZZER_ACTIVE, 0);
//     NVS_write_uint16(NVS_BRAKE_OPEN_TIME, 0);
//     NVS_write_uint16(NVS_RECOV_SAFETY_TRIG_TIME, 0);
//     NVS_write_uint16(NVS_RECOV_OPEN_TIME, 0);
// }

static bool read_settings_from_flash(void) {
    settings_t * settings = SETI_get_settings();
    uint8_t res8 = 0;
    uint16_t res16 = 0;
    NVSResult res = NVS_OK;

    res |= NVS_read_uint8(NVS_TEST_MODE, &res8);
    settings->test_mode = res8;

    res |= NVS_read_uint8(NVS_BUZZER_ACTIVE, &res8);
    settings->buzzer_active = res8;

    res |= NVS_read_uint8(NVS_ALPHA, &res8);
    settings->alpha = res8 / 100.0;

    res |= NVS_read_uint8(NVS_BETA, &res8);
    settings->beta = res8 / 100.0;

    res |= NVS_read_uint16(NVS_BRAKE_CLOSE_ANGLE, &res16);
    settings->brake_close_angle = res16;

    res |= NVS_read_uint16(NVS_BRAKE_OPEN_ANGLE, &res16);
    settings->brake_open_angle = res16;

    res |= NVS_read_uint16(NVS_RECOVERY_CLOSE_ANGLE, &res16);
    settings->recovery_close_angle = res16;

    res |= NVS_read_uint16(NVS_RECOVERY_OPEN_ANGLE, &res16);
    settings->recovery_open_angle = res16;

    res |= NVS_read_uint16(NVS_BRAKE_OPEN_TIME, &res16);
    settings->brake_open_time = res16;

    res |= NVS_read_uint16(NVS_RECOV_SAFETY_TRIG_TIME, &res16);
    settings->recovery_safety_trigger = res16;

    res |= NVS_read_uint16(NVS_RECOV_OPEN_TIME, &res16);
    settings->recovery_open_time = res16;

    return res == NVS_OK ? true : false;
}

/**************************** ERRORS ***************************/

static void err_handling(char *name) {
    ESP_LOGE(TAG, "Init error -> %s", name);
    BUZZER_set_level(0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    BUZZER_set_level(1);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    esp_restart();
}

static inline void ERR_CHECK_BOOL(bool status, char *name) {
  if (status == false) {
    err_handling(name);
  }
}

static inline void ERR_CHECK_STATUS(uint32_t status, char *name) {
  if (status != 0) {
    err_handling(name);
  }
}

void init_task(void *arg) {
    BUZZER_init(PCB_BUZZER);
    BUZZER_set_level(1);
    ERR_CHECK_BOOL(IGNITER_init(PCB_IGNITER1), "INGITER");

    ERR_CHECK_BOOL(SPI_init(&sd_spi, HSPI_HOST, PCB_MOSI, PCB_MISO, PCB_SCK), "SPI");
    ERR_CHECK_BOOL(I2C_init(&i2c_sensors, I2C_NUM_1, PCB_SDA, PCB_SCL), "I2C");
    SM_init();
    ERR_CHECK_STATUS(SM_set_states(states_conf, sizeof(states_conf) / sizeof(states_conf[0])),
              "State machine set state");
    ERR_CHECK_BOOL(SM_run() == SM_OK ? true : false, "State machine run");

    ERR_CHECK_STATUS(NVS_init(), "NVS");
    ERR_CHECK_BOOL(read_settings_from_flash(), "NVS read");
    ERR_CHECK_BOOL(SD_init(&sd_card, sd_spi.spi_host, PCB_SD_CS, MOUNT_POINT), "SD_init");

    ERR_CHECK_STATUS(voltageMeasureInit(&vMes, BATT_ADC_CHANNEL, BATT_ADC_CAL), "V measure");
    watchdog_init(100, 8000, TASK_PRIORITY_HIGH, &wh);
    ERR_CHECK_BOOL(event_loop_init(), "event loop");
    ERR_CHECK_BOOL(event_loop_register(), "event loop register");

    if (SETI_get_settings()->test_mode == TEST_MODE_ON) {
        ESP_LOGI(TAG, "Running in test mode");
        ERR_CHECK_STATUS(NVS_write_uint8(NVS_TEST_MODE, TEST_MODE_OFF), "NVS write");
        ERR_CHECK_BOOL(UART_init(&uart, UART_NUM_0, PCB_TX, PCB_RX, 115200), "Uart");
        ERR_CHECK_BOOL(rtos_test_mode_init(), "RTOS init");
    } else {
        ESP_LOGI(TAG, "Running in normal mode");

        ERR_CHECK_BOOL(LSM6DS3_init(&acc_sensor, 0x6B, i2c_num1_write, i2c_num1_read), "LSM6DS3");
        ERR_CHECK_BOOL(LSM6DS3_set_acc_scale(&acc_sensor, LSM6DS3_ACC_16G), "LSM6DSA3 acc scale");
        ERR_CHECK_BOOL(LSM6DS3_set_gyro_scale(&acc_sensor, LSM6DS3_GYRO_2000),
                        "LSM6DSA3 gyro scale");
        ERR_CHECK_STATUS(LPS25HInit(&press_sensor, I2C_NUM_1, LPS25H_I2C_ADDR_SA0_H), "LPS25H");
        ERR_CHECK_STATUS(LPS25HStdConf(&press_sensor), "LPS25HB conf");
        ERR_CHECK_BOOL(rtos_init(), "RTOS init");
        ERR_CHECK_BOOL(console_init(), "CLI");
        ERR_CHECK_BOOL(console_register_commands(console_commands,
            sizeof(console_commands)/sizeof(console_commands[0])), "CLI register");
    }

    BUZZER_set_level(0);
    ERR_CHECK_BOOL(RECOV_SERVO_init(), "Recovery servo init");
    ERR_CHECK_BOOL(BRAKE_SERVO_init(), "Brake servo init");
    ERR_CHECK_BOOL(RECOV_SERVO_move(SETI_get_settings()->recovery_close_angle),
                  "Recovery servo move");
    ERR_CHECK_BOOL(BRAKE_SERVO_move(SETI_get_settings()->brake_close_angle),
                  "Brake servo init");

    if (SETI_get_settings()->buzzer_active == true) {
        TIMER_start(BUZZER_TIMER, 1000, TIMER_PERIODIC, TIMER_CB_buzzer_change, NULL);
    }
    ESP_LOGI(TAG, "Deleting init task");
    vTaskDelete(NULL);
}
