idf_component_register(SRCS "sensor.c" "voltageMeasure.c" "LPS25H.c" "LSM6DS3.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock sensor driver)

