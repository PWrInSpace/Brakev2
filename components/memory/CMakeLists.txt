idf_component_register(SRCS "flash_nvs.c" "flash.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock memory spi_flash spiffs nvs_flash)