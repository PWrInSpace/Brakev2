#include "rtos_tasks.h"
#include "utils.h"
#include "state_machine.h"

#define TAG "MAIN TASK"

static esp_event_loop_handle_t event_handle;

static struct {
    uint32_t timestamp;
    uint8_t state;
} data;

ESP_EVENT_DEFINE_BASE(TASK_EVENTS);


static void update_data(void) {
    data.state = SM_get_current_state();
    data.timestamp = get_time_ms();
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
    // send to sd
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