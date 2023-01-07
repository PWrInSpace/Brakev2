// Copyright 2022 PWrInSpace, Kuba

#include "rtos_tasks.h"
#include "console.h"
#include "console_commands.h"
#include "state_machine.h"

#include "config.h"

#define TAG "INIT"

rtos_t rtos;
spi_t sd_spi;
i2c_t i2c_sensors;
sd_card_t sd_card;
LSM6DS3_t acc_sensor;

esp_console_cmd_t console_commands[] = {
    {"test", "test1234", NULL, CLI_test, NULL},
    {"sm-state", "Get current state", NULL, CLI_state_machine_get_state, NULL},
    {"reset-dev", "Software reset", NULL, CLI_reset_device, NULL},
    {"reset-reason", "Get reset reason", NULL, CLI_reset_reason, NULL},
};


bool rtos_init(void) {
    rtos.data_to_memory = xQueueCreate(20, sizeof(data_to_memory_task_t));

    if (rtos.data_to_memory == NULL) {
        return false;
    }

    xTaskCreatePinnedToCore(sensor_task, "sensor_task", 8000, NULL,
                            10, &rtos.sensor_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL,
                            10, &rtos.main_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL,
                            5, &rtos.memory_task, APP_CPU_NUM);

    if (rtos.sensor_task == NULL || rtos.main_task == NULL || rtos.memory_task == NULL) {
        return false;
    }

    return true;
}

bool rtos_test_mode_init(void) {
    rtos.data_to_memory = xQueueCreate(20, sizeof(data_to_memory_task_t));

    if (rtos.data_to_memory == NULL) {
        return false;
    }

    xTaskCreatePinnedToCore(sensor_task, "sensor_task", 8000, NULL,
                            10, &rtos.sensor_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL,
                            10, &rtos.main_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL,
                            5, &rtos.memory_task, APP_CPU_NUM);

    if (rtos.sensor_task == NULL || rtos.main_task == NULL || rtos.memory_task == NULL) {
        return false;
    }

    return true;
}

static bool i2c_num1_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (i2c_master_write_read_device(I2C_NUM_1, dev_addr, &reg_addr, 1, data, len,
                                     pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

static bool i2c_num1_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    uint8_t tmp[2] = {reg_addr, data[0]};
    if (i2c_master_write_to_device(I2C_NUM_1, dev_addr, tmp, sizeof(tmp),
                                   pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

void init_task(void *arg) {
    bool ret;
    SPI_init(&sd_spi, HSPI_HOST, PCB_MOSI, PCB_MISO, PCB_SCK);
    I2C_init(&i2c_sensors, I2C_NUM_1, PCB_SDA, PCB_SCL);
    SM_init();

    LSM6DS3_init(&acc_sensor, 0x6B, i2c_num1_write, i2c_num1_read);
    LSM6DS3_set_acc_scale(&acc_sensor, LSM6DS3_ACC_16G);
    LSM6DS3_set_gyro_scale(&acc_sensor, LSM6DS3_GYRO_2000);
    SD_init(&sd_card, sd_spi.spi_host, PCB_SD_CS, MOUNT_POINT);
    console_init();
    console_register_commands(console_commands,
        sizeof(console_commands)/sizeof(console_commands[0]));
    event_loop_init();
    event_loop_register();
    rtos_init();

    vTaskDelete(NULL);
}
