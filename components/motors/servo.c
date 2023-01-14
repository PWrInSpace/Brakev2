// Copyright 2022 PWrInSpace, Kuba

#include "servo.h"

static inline uint32_t angle_to_compare(servo_t *servo, int angle) {
    return (angle - servo->config.min_degree) *
           (servo->config.max_pulse_width_us - servo->config.min_pulse_width_us) /
           (servo->config.max_degree - servo->config.min_degree) +
           servo->config.min_pulse_width_us;
}


static bool initialize_timer(servo_t *servo) {
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };

    if (mcpwm_new_timer(&timer_config, &servo->timer) != ESP_OK) {
        return false;
    }
    return true;
}

static bool initialize_operator(servo_t *servo) {
    mcpwm_operator_config_t operator_config = {
        .group_id = 0,  // operator must be in the same group to the timer
    };

    if (mcpwm_new_operator(&operator_config, &servo->operator) != ESP_OK) {
        return false;
    }
    return true;
}

static bool connect_operator_with_timer(servo_t *servo) {
    if (mcpwm_operator_connect_timer(servo->operator, servo->timer) != ESP_OK) {
        return false;
    }
    return true;
}

static bool initialize_comparator(servo_t *servo) {
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };

    if (mcpwm_new_comparator(servo->operator, &comparator_config, &servo->comparator) != ESP_OK) {
        return false;
    }
    return true;
}

static bool initialize_generator(servo_t *servo) {
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = servo->config.pwm_pin,
    };

    if (mcpwm_new_generator(servo->operator, &generator_config, &servo->generator) != ESP_OK) {
        return false;
    }

    return true;
}

static bool set_initial_compare_value(servo_t *servo) {
    esp_err_t ret;
    ret = mcpwm_comparator_set_compare_value(
        servo->comparator,
        angle_to_compare(servo, 0));

    return ret == ESP_OK ? true : false;
}

static bool set_generator_acitions(servo_t *servo) {
     esp_err_t ret;
     ret = mcpwm_generator_set_actions_on_timer_event(
        servo->generator,
        MCPWM_GEN_TIMER_EVENT_ACTION(
            MCPWM_TIMER_DIRECTION_UP,
            MCPWM_TIMER_EVENT_EMPTY,
            MCPWM_GEN_ACTION_HIGH),
        MCPWM_GEN_TIMER_EVENT_ACTION_END());
    if (ret != ESP_OK) {
        return false;
    }

    // go low on compare threshold
    ret = mcpwm_generator_set_actions_on_compare_event(
        servo->generator,
        MCPWM_GEN_COMPARE_EVENT_ACTION(
            MCPWM_TIMER_DIRECTION_UP,
            servo->comparator,
            MCPWM_GEN_ACTION_LOW),
        MCPWM_GEN_COMPARE_EVENT_ACTION_END());

    return ret == ESP_OK ? true : false;
}

static bool start_timer(servo_t *servo) {
    if (mcpwm_timer_enable(servo->timer) != ESP_OK) {
        return false;
    }
    if (mcpwm_timer_start_stop(servo->timer, MCPWM_TIMER_START_NO_STOP) != ESP_OK) {
        return false;
    }

    return true;
}

bool SERVO_init(servo_t *servo, servo_config_t config) {
    servo->config = config;
    if (initialize_timer(servo) == false) return false;
    if (initialize_operator(servo) == false) return false;
    if (connect_operator_with_timer(servo) == false) return false;
    if (initialize_comparator(servo) == false) return false;
    if (initialize_generator(servo) == false) return false;
    if (set_initial_compare_value(servo) == false) return false;
    if (set_generator_acitions(servo) == false) return false;
    if (start_timer(servo) == false) return false;

    return true;
}


bool SERVO_move(servo_t *servo, int16_t angle) {
    esp_err_t ret;
    ret = mcpwm_comparator_set_compare_value(
        servo->comparator,
        angle_to_compare(servo, angle));
    return ret == ESP_OK ? true : false;
}
