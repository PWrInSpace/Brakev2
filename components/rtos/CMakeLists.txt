idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    REQUIRES cmock utils sys esp_event driver main memory)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")