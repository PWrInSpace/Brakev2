idf_component_register(SRCS "main.c" "console.c" "sdcard.c"
                    INCLUDE_DIRS "")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
spiffs_create_partition_image(storage ../spiffs_data FLASH_IN_PROJECT)