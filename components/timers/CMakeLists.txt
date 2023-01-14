idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_timer gpio)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")