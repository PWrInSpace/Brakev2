// Copyright 2022 PWr in Space, Krzysztof Gliwiński

#include "sdCard.h"

static struct{
  spi_bus_config_t spiConfig;
  sdmmc_host_t host;
  esp_vfs_fat_mount_config_t fatConfig;
  sdmmc_card_t *card;
} sdCard;