idf_component_register(SRCS "main.c" "console.c" "sdcard.c"
                    INCLUDE_DIRS "")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
