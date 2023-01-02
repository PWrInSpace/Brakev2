// Copyright 2022 PWr in Space, Krzysztof Gliwiński

#include "sdCard.h"

static struct {
  spi_bus_config_t spiConfig;
  sdmmc_host_t host;
  esp_vfs_fat_mount_config_t fatConfig;
  sdmmc_card_t *card;
} sdCard;

spi_bus_config_t sdCardSPIConfig(uint8_t miso, uint8_t mosi, uint8_t clk,
                                 int max_transfer_sz) {
  spi_bus_config_t bus_cfg = {
      .mosi_io_num = PIN_NUM_MOSI,
      .miso_io_num = PIN_NUM_MISO,
      .sclk_io_num = PIN_NUM_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4000,
  };
  return bus_cfg;
}