// Copyright 2022 PWrInSpace, Kuba

#include "rtos_tasks.h"
#include "console.h"
#include "console_commands.h"
#include "state_machine.h"

#include "config.h"

#define TAG "INIT"

#define BATT_ADC_CAL 3.3f

rtos_t rtos;
spi_t sd_spi;
i2c_t i2c_sensors;
uart_t uart;
sd_card_t sd_card;
LSM6DS3_t acc_sensor;
LPS25H press_sensor;
VoltageMeasure vMes;

esp_console_cmd_t console_commands[] = {
    {"test", "test1234", NULL, CLI_test, NULL},
    {"sm-state", "Get current state", NULL, CLI_state_machine_get_state, NULL},
    {"reset-dev", "Software reset", NULL, CLI_reset_device, NULL},
    {"reset-reason", "Get reset reason", NULL, CLI_reset_reason, NULL},
    {"brake-servo-move", "Move brake servo", NULL, CLI_brake_move, NULL},
    {"recov-servo-move", "Move recovery servo", NULL, CLI_recov_move, NULL}
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

    xTaskCreatePinnedToCore(test_mode_task, "sensor_task", 8000, NULL,
                            TASK_PRIORITY_HIGH, &rtos.sensor_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL,
                            TASK_PRIORITY_HIGH, &rtos.main_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL,
                            TASK_PRIORITY_MID, &rtos.memory_task, APP_CPU_NUM);

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

    RECOV_SERVO_init();
    BRAKE_SERVO_init();

    LSM6DS3_init(&acc_sensor, 0x6B, i2c_num1_write, i2c_num1_read);
    LSM6DS3_set_acc_scale(&acc_sensor, LSM6DS3_ACC_16G);
    LSM6DS3_set_gyro_scale(&acc_sensor, LSM6DS3_GYRO_2000);
    LPS25HInit(&press_sensor, I2C_NUM_1, LPS25H_I2C_ADDR_SA0_H);
    LPS25HStdConf(&press_sensor);
    voltageMeasureInit(&vMes, BATT_ADC_CHANNEL, BATT_ADC_CAL);
    SD_init(&sd_card, sd_spi.spi_host, PCB_SD_CS, MOUNT_POINT);
    event_loop_init();
    event_loop_register();
    console_init();
    console_register_commands(console_commands,
        sizeof(console_commands)/sizeof(console_commands[0]));
    rtos_init();

    // UART_init(&uart, UART_NUM_0, PCB_TX, PCB_RX, 115200);
    // rtos_test_mode_init();

    vTaskDelete(NULL);
}

