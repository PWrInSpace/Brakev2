// Copyright 2022 PWrInSpace

#include "state_machine.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <freertos/semphr.h>
#include <string.h>
#include <stdbool.h>
#include "esp_log.h"

static struct {
    state_t states[SM_MAX - SM_ZERO];
    uint8_t states_number;
    uint8_t added_states;
    uint8_t current_state;
    end_looped_function end_function;
    uint32_t end_fct_frq_ms;
    TaskHandle_t state_task;
    SemaphoreHandle_t current_state_mutex;
} sm;

void SM_init() {
    memset(sm.states, 0, sizeof(sm.states));
    sm.states_number = SM_MAX - SM_ZERO - 1;
    sm.added_states = 0;
    sm.current_state = 0;
    sm.end_function = NULL;
    sm.end_fct_frq_ms = 0;
    sm.state_task = NULL;
    sm.current_state_mutex = xSemaphoreCreateMutex();
}

SM_Response SM_add_state(States state_name, state_change_callback callback, void* arg) {
    if (sm.added_states > sm.states_number) {
        return SM_MEMORY_ERROR;
    }

    sm.states[sm.added_states].callback = callback;
    sm.states[sm.added_states].arg = arg;
    sm.states[sm.added_states].state_name = state_name;
    sm.added_states += 1;

    return SM_OK;
}

SM_Response SM_set_end_function(end_looped_function function, uint32_t freq_ms) {
    if (function == NULL) {
        return SM_NULL_FUNCTION;
    }

    sm.end_function = function;
    sm.end_fct_frq_ms = freq_ms;

    return SM_OK;
}

static bool SM_check_new_state(States new_state) {
    assert(sm.current_state < sm.states_number);
    if (sm.current_state >= sm.states_number) {
        return false;
    }

    if (sm.state_task == NULL) {
        return false;
    }

    if (sm.states[sm.current_state + 1].state_name != new_state) {
        return false;
    }

    return true;
}

SM_Response SM_change_state(States new_state) {
    xSemaphoreTake(sm.current_state_mutex, portMAX_DELAY);
    if (SM_check_new_state(new_state)) {
      sm.current_state += 1;
      xTaskNotifyGive(sm.state_task);
      return SM_OK;
    }
    xSemaphoreGive(sm.current_state_mutex);

    return SM_STATE_CHANGE_ERROR;
}

SM_Response SM_change_state_ISR(States new_state) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreTake(sm.current_state_mutex, portMAX_DELAY);
    if (SM_check_new_state(new_state)) {
      sm.current_state += 1;
      vTaskNotifyGiveFromISR(sm.state_task, &xHigherPriorityTaskWoken);
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
      return SM_OK;
    }
    xSemaphoreGive(sm.current_state_mutex);

    return SM_STATE_CHANGE_ERROR;
}

States SM_get_current_state(void) {
    States current_state;

    xSemaphoreTake(sm.current_state_mutex, portMAX_DELAY);
    current_state = sm.states[sm.current_state].state_name;
    xSemaphoreGive(sm.current_state_mutex);

    return current_state;
}

States SM_get_current_state_ISR(void) {
    States current_state;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreTakeFromISR(sm.current_state_mutex, &xHigherPriorityTaskWoken);
    current_state = sm.states[sm.current_state].state_name;
    xSemaphoreGiveFromISR(sm.current_state_mutex, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return current_state;
}

static void SM_loop(void *arg) {
    bool sm_completed = false;

    while (1) {
        assert(sm.current_state < sm.states_number);
        if (ulTaskNotifyTake(pdTRUE, 0)) {
            xSemaphoreTake(sm.current_state_mutex, pdTRUE);
            if (sm.states[sm.current_state].callback != NULL) {
                sm.states[sm.current_state].callback(sm.states[sm.current_state].arg);
            }

            if (sm.current_state + 1 >= sm.states_number) {
                sm_completed = true;
            }

            xSemaphoreGive(sm.current_state_mutex);
        } else if (sm_completed == true) {  // end function
            if (sm.end_function != NULL) {
                sm.end_function();
                vTaskDelay(sm.end_fct_frq_ms / portTICK_PERIOD_MS - 10);
            }
        }

        vTaskDelay(10);
    }
}

SM_Response SM_run(void) {
    assert(sm.added_states == sm.states_number);
    if (sm.added_states != sm.states_number) {
        return SM_UNUSED_STATE;
    }

    xTaskCreatePinnedToCore(SM_loop, "SM", 8000, NULL, 10, &sm.state_task, APP_CPU_NUM);

    assert(sm.state_task != NULL);
    if (sm.state_task == NULL) {
        return SM_RUN_ERROR;
    }

    xTaskNotifyGive(sm.state_task);
    return SM_OK;
}

