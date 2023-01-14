idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    REQUIRES cmock driver utils)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")