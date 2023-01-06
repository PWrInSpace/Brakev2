#include "brejk_system.h"
#include "utils.h"
#include "state_machine.h"

#define TAG "MAIN TASK"

extern rtos_t rtos;

static rocket_data_t main_data;
static esp_event_loop_handle_t event_handle;

ESP_EVENT_DEFINE_BASE(TASK_EVENTS);

static data_to_memory_task_t create_data_to_memory_struct(void) {
    data_to_memory_task_t data_to_mem;
    data_to_mem.data = main_data;
    if (main_data.state < 2) {
        data_to_mem.save_option = SAVE_TO_SD;
    } else if (main_data.state < 5) {
        data_to_mem.save_option = SAVE_TO_BOTH;
    }

    return data_to_mem;
}

static void update_data(void) {
    main_data.state = SM_get_current_state();
    main_data.up_time = get_time_ms();
}

/********************** EVENTS ************************/

static void save_data_event(void *h_args, esp_event_base_t base, int32_t id, void *data) {
    ESP_LOGI(TAG, "EVENT HELLO WORLD!!!!!!");
    // event triggered by timer
    // check when last time data was saved
}

static void sensors_new_data_event(void *h_arg, esp_event_base_t, int32_t id, void *data) {
    ESP_LOGI(TAG, "NEW DATA EVENT");
    // get data
    // update data
    update_data();
    if (main_data.state < 6) {
        data_to_memory_task_t data_to_memory = create_data_to_memory_struct();
        if (xQueueSend(rtos.data_to_memory, (void*) &data_to_memory, 0) == pdFALSE) {
            ESP_LOGE(TAG, "UNABLE TO SEND DATA TO MEMORY TASK");
        }
    }
    // set brejk
}

static void sensors_high_acc_event(void *h_arg, esp_event_base_t, int32_t id, void *data) {
    ESP_LOGI(TAG, "HIGH ACCELERATION");
    // if (state == LAUNCHPAD) {
    //     SM_change_state(ASCENT)
    // }
}

/********************** EVENT LOOP ************************/

bool event_loop_init(void) {
    esp_err_t res;
    esp_event_loop_args_t event_loop = {
        .queue_size = 15,
        .task_name = NULL
    };

    res = esp_event_loop_create(&event_loop, &event_handle);
    return res == ESP_OK ? true : false;
}

bool event_loop_register(void) {
    esp_err_t res = ESP_OK;
    res |= esp_event_handler_instance_register_with(event_handle,
                                                    TASK_EVENTS,
                                                    SAVE_DATA_EVENT,
                                                    save_data_event,
                                                    NULL,
                                                    NULL);

    res |= esp_event_handler_instance_register_with(event_handle,
                                                    TASK_EVENTS,
                                                    SENSORS_NEW_DATA_EVENT,
                                                    sensors_new_data_event,
                                                    NULL,
                                                    NULL);

    res |= esp_event_handler_instance_register_with(event_handle,
                                                    TASK_EVENTS,
                                                    SENSORS_HIGH_ACC_EVENT,
                                                    sensors_high_acc_event,
                                                    NULL,
                                                    NULL);


    return res == ESP_OK ? true : false;
}

esp_event_loop_handle_t event_get_handle(void) {
    return event_handle;
}

/********************** MAIN TASK ************************/

void main_task(void *arg) {

    while (1) {
        ESP_LOGI(TAG, "application_task: running application task");
        esp_event_loop_run(event_handle, 1000);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}