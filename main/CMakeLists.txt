idf_component_register(SRCS "main.c" "console.c" "sdcard.c" "rtos_main_task.c" "rtos_memory_task.c" "brejk_system.c" "rtos_sensor_task.c" "rtos_test_mode_task.c"
                    INCLUDE_DIRS "")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
