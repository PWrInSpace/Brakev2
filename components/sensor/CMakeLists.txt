idf_component_register(SRCS "voltageMeasure.c" "LPS25H.c" "LSM6DS3.c" "alpha_beta_filter.c"
                    INCLUDE_DIRS "."
                    REQUIRES cmock sensor driver esp_adc)

