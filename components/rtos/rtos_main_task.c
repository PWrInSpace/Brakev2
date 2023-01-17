// Copyright 2022 PWrInSpace, Kuba

#include "rtos_tasks.h"
#include "state_machine.h"
#include "utils.h"

#define TAG "MAIN TASK"

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

static void sensors_high_acc_event(void *h_arg, esp_event_base_t, int32_t id,
                                   void *data) {
    ESP_LOGI(TAG, "HIGH ACCELERATION");
    if (SM_change_state(ASCENT) != SM_OK) {
        ESP_LOGE(TAG, "UNABLE TO CHANGE STATE TO ASCENT");
        return;
    }

    BUZZER_set_level(1);
    TIMER_start(BRAKE_TIMER, SETI_get_settings()->brake_open_time,
               TIMER_ONE_SHOT, TIMER_CB_brake_open, NULL);
    TIMER_start(RECOV_SAFETY_TRIGGER, SETI_get_settings()->recovery_safety_trigger,
               TIMER_ONE_SHOT, TIMER_CB_recov_trig, NULL);
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 500, TIMER_ONE_SHOT, TIMER_CB_buzzer_low, NULL);
}

static void apogee_event(void *h_arg, esp_event_base_t, int32_t id,
                         void *data) {
    ESP_LOGI(TAG, "Apogee event");
    if (SM_change_state(DESCENT) != SM_OK) {
        ESP_LOGE(TAG, "Unable to change state to BRAKE");
        return;
    }

    settings_t *settings = SETI_get_settings();
    RECOV_SERVO_move(SETI_get_settings()->recovery_open_angle);
    TIMER_start(IGNITER_TIMER_ON, settings->recovery_safety_trigger, TIMER_ONE_SHOT,
              TIMER_CB_igniter_high, NULL);
    TIMER_start(IGNITER_TIMER_OFF,
              settings->recovery_safety_trigger + settings->recovery_open_time,
              TIMER_ONE_SHOT, TIMER_CB_igniter_low, NULL);
    TIMER_start(BUZZER_TIMER, 3000, TIMER_ONE_SHOT, TIMER_CB_brake_close, NULL);

    BUZZER_set_level(1);
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 1000, TIMER_ONE_SHOT, TIMER_CB_buzzer_low, NULL);
}

static void brake_event(void *h_arg, esp_event_base_t, int32_t id, void *data) {
    ESP_LOGI(TAG, "Brake event");
    if (SM_change_state(BRAKE) != SM_OK) {
        ESP_LOGE(TAG, "Unable to change state to BRAKE");
        return;
    }

    BRAKE_SERVO_move(SETI_get_settings()->brake_open_angle);
    BUZZER_set_level(1);
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 500, TIMER_ONE_SHOT, TIMER_CB_buzzer_low, NULL);
}

static void touchdown_event(void *h_arg, esp_event_base_t, int32_t id, void *data) {
    if (SM_change_state(GROUND) != SM_OK) {
        ESP_LOGE(TAG, "UNABLE TO CHANGE STATE TO GROUND");
        return;
    }
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 1000, TIMER_PERIODIC, TIMER_CB_buzzer_change, NULL);
}

static void sensors_new_data_event(void *h_arg, esp_event_base_t, int32_t id,
                                   void *data) {
  ESP_LOGI(TAG, "NEW DATA EVENT");
  // get data
  // update data
  update_data();

  if (main_data.state < 6) {
    data_to_memory_task_t data_to_memory = create_data_to_memory_struct();
    if (xQueueSend(rtos.data_to_memory, (void *)&data_to_memory, 0) ==
        pdFALSE) {
      ESP_LOGE(TAG, "UNABLE TO SEND DATA TO MEMORY TASK");
    }
  }
  // set brejk
}

/********************** EVENT LOOP ************************/

bool event_loop_init(void) {
  esp_err_t res;
  esp_event_loop_args_t event_loop = {.queue_size = 15, .task_name = NULL};

  res = esp_event_loop_create(&event_loop, &event_handle);
  return res == ESP_OK ? true : false;
}

bool event_loop_register(void) {
  esp_err_t res = ESP_OK;
  res |= esp_event_handler_instance_register_with(event_handle, TASK_EVENTS,
                                                  APOGEE_EVENT,
                                                  apogee_event, NULL, NULL);

  res |= esp_event_handler_instance_register_with(
      event_handle, TASK_EVENTS, SENSORS_NEW_DATA_EVENT, sensors_new_data_event,
      NULL, NULL);

  res |= esp_event_handler_instance_register_with(
      event_handle, TASK_EVENTS, SENSORS_HIGH_ACC_EVENT, sensors_high_acc_event,
      NULL, NULL);
  res |= esp_event_handler_instance_register_with(
      event_handle, TASK_EVENTS, TOUCHDOWN_EVENT, touchdown_event,
      NULL, NULL);
  res |= esp_event_handler_instance_register_with(
      event_handle, TASK_EVENTS, BRAKE_OPEN_EVENT, brake_event,
      NULL, NULL);

  return res == ESP_OK ? true : false;
}

esp_event_loop_handle_t event_get_handle(void) { return event_handle; }

/********************** MAIN TASK ************************/
void main_task(void *arg) {
    while (1) {
        esp_event_loop_run(event_handle, 1000);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
