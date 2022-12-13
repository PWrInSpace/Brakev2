idf_component_register(SRCS "sensor.c" "voltageMeasure.c" "LPS25H.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock sensor driver)

