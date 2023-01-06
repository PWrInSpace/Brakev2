#include "brejk_system.h"
#include "config.h"

rtos_t rtos;
i2c_t sensors_i2c;
spi_t spi;


/********** RTOS ***********/
bool rtos_init(void) {
    rtos.data_to_memory = xQueueCreate(20, sizeof(data_to_memory_task_t));

    if (rtos.data_to_memory == NULL) {
        return false;
    }

    xTaskCreatePinnedToCore(sensor_task, "sensor_task", 8000, NULL, 10, &rtos.sensor_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL, 10, &rtos.main_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL, 5, &rtos.memory_task, APP_CPU_NUM);

    if (rtos.sensor_task == NULL || rtos.main_task == NULL || rtos.memory_task == NULL) {
        return false;
    }

    return true;
}

bool rtos_test_mode_init(void) {
    rtos.data_to_memory = xQueueCreate(20, 20);

    if (rtos.data_to_memory == NULL) {
        return false;
    }

    xTaskCreatePinnedToCore(sensor_task, "sensor_task", 8000, NULL, 10, &rtos.sensor_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(main_task, "main_task", 8000, NULL, 10, &rtos.main_task, APP_CPU_NUM);
    xTaskCreatePinnedToCore(memory_task, "memory_task", 8000, NULL, 5, &rtos.memory_task, APP_CPU_NUM);

    if (rtos.sensor_task == NULL || rtos.main_task == NULL || rtos.memory_task == NULL) {
        return false;
    }

    return true;
}

rtos_t* get_rtos_struct(void) {
    return &rtos;
}

/************** SPI *******************/

bool spi_init(void) {
    esp_err_t ret;
    spi.spi_bus.mosi_io_num = PCB_MOSI;
    spi.spi_bus.miso_io_num = PCB_MISO;
    spi.spi_bus.sclk_io_num = PCB_SCK;
    spi.spi_bus.quadwp_io_num = -1;
    spi.spi_bus.quadhd_io_num = -1;
    spi.spi_bus.max_transfer_sz = 4000;
    spi.spi_host = HSPI_HOST;

    ret = spi_bus_initialize(spi.spi_host, &spi.spi_bus, SDSPI_DEFAULT_DMA);
    return ret == ESP_OK ? true : false;
}

spi_t* get_spi_struct(void) {
    return &spi;
}

/************** I2C *******************/

bool i2c_sensor_init(void) {
    esp_err_t res;
    sensors_i2c.i2c_config.mode = I2C_MODE_MASTER;
    sensors_i2c.i2c_config.sda_io_num = 21;
    sensors_i2c.i2c_config.scl_io_num = 22;
    sensors_i2c.i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    sensors_i2c.i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    sensors_i2c.i2c_config.master.clk_speed = 400000;
    sensors_i2c.i2c_config.clk_flags = 0;

    res = i2c_param_config(I2C_NUM_1, &sensors_i2c.i2c_config);
    ESP_ERROR_CHECK(res);
    res = i2c_driver_install(I2C_NUM_1, sensors_i2c.i2c_config.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);
    return res == ESP_OK ? true : false;
}

bool i2c_num1_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    if (i2c_master_write_read_device(I2C_NUM_1, dev_addr, &reg_addr, 1, data, len,
                                     pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

bool i2c_num1_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    assert(len == 1);
    uint8_t tmp[2] = {reg_addr, data[0]};
    if (i2c_master_write_to_device(I2C_NUM_1, dev_addr, tmp, sizeof(tmp),
        pdMS_TO_TICKS(100)) == ESP_OK) {
        return true;
    }

    return false;
}

i2c_t* get_i2c_struct(void) {
    return &sensors_i2c;
}