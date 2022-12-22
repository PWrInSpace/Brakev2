// Copyright 2022 PWr in Space, Krzysztof Gliwiński

#pragma once

#include <string.h>

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

/*!
    \file contains SD Card SPI interface
*/

#define MOUNT_POINT "/sdcard"

spi_bus_config_t sdCardSPIConfig(uint8_t miso, uint8_t mosi, uint8_t clk, int max_transfer_sz);

sdmmc_host_t sdCardMMCConfig();

esp_vfs_fat_mount_config_t sdCardFATConfig();

esp_err_t sdCardInit(spi_bus_config_t spiConfigPtr,
                     sdmmc_host_t hostVar,
                     esp_vfs_fat_mount_config_t fatConfigPtr,
                     sdmmc_card_t *card);
