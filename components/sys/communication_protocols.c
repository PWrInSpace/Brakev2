// Copyright 2022 PWrInSpace

#include "communication_protocols.h"

// static esp_err_t spi_init(spi_buss_config_t *cnfg, spi_host_device_t *host) {
//     esp_err_t ret;
//     cnfg->spi_bus.mosi_io_num = PIN_NUM_MOSI,
//     cnfg->spi_bus.miso_io_num = PIN_NUM_MISO,
//     cnfg->spi_bus.sclk_io_num = PIN_NUM_CLK,
//     cnfg->spi_bus.quadwp_io_num = -1,
//     cnfg->spi_bus.quadhd_io_num = -1,
//     cnfg->spi_bus.max_transfer_sz = 4000,
//     *host = HSPI_HOST;

//     ret = spi_bus_initialize(host, &cnfg->spi_bus, SDSPI_DEFAULT_DMA);
//     return ret;
// }

// static esp_err_t i2c_sensor_init(i2c_config_t *cnfg, i2c_port_t port, uint8_t sda, uint8_t scl) {
//     esp_err_t res;
//     cnfg->i2c_config.mode = I2C_MODE_MASTER;
//     cnfg->i2c_config.sda_io_num = sda;
//     cnfg->i2c_config.scl_io_num = scl;
//     cnfg->i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
//     cnfg->i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
//     cnfg->i2c_config.master.clk_speed = 400000;
//     cnfg->i2c_config.clk_flags = 0;

//     res = i2c_param_config(port, &cnfg->i2c_config);
//     ESP_ERROR_CHECK(res);
//     res = i2c_driver_install(port, cnfg->i2c_config.mode, 0, 0, 0);
//     ESP_ERROR_CHECK(res);
//     return res;
// }

// static bool i2c_num1_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
//     if (i2c_master_write_read_device(I2C_NUM_1, dev_addr, &reg_addr, 1, data, len,
//                                      pdMS_TO_TICKS(100)) == ESP_OK) {
//         return true;
//     }

//     return false;
// }

// static bool i2c_num1_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
//     assert(len == 1);
//     uint8_t tmp[2] = {reg_addr, data[0]};
//     if (i2c_master_write_to_device(I2C_NUM_1, dev_addr, tmp, sizeof(tmp),
//         pdMS_TO_TICKS(100)) == ESP_OK) {
//         return true;
//     }

//     return false;
// }
