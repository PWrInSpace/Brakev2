// Copyright 2022 PWrInSpace, Kuba

#include "rtos_tasks.h"
#include "state_machine.h"
#include "utils.h"

#define TAG "MAIN TASK"

static rocket_data_t main_data;
static esp_event_loop_handle_t event_handle;

ESP_EVENT_DEFINE_BASE(TASK_EVENTS);

static void update_data(void) {
  main_data.state = SM_get_current_state();
  main_data.up_time = get_time_ms();
  main_data.flight_time = FLIGHT_TIME_get_time(get_time_ms());
}

/********************** EVENTS ************************/

static void wakeup_flash_task(void) {
    xTaskNotifyGive(rtos.flash_task);
}

static void sensors_high_acc_event(void *h_arg, esp_event_base_t, int32_t id,
                                   void *data) {
    ESP_LOGD(TAG, "HIGH ACCELERATION");
    if (SM_change_state(ASCENT) != SM_OK) {
        ESP_LOGE(TAG, "UNABLE TO CHANGE STATE TO ASCENT");
        return;
    }
    wakeup_flash_task();
    FLIGHT_TIME_start(get_time_ms());
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
    ESP_LOGD(TAG, "Apogee event");
    if (SM_change_state(DESCENT) != SM_OK) {
        ESP_LOGE(TAG, "Unable to change state to DESCENT");
        return;
    }

    ESP_LOGI(TAG, "Apogee deteceted, altitude: %.2f\t time: %d",
      main_data.sensors.filtered.height, FLIGHT_TIME_get_time(get_time_ms()));

    settings_t *settings = SETI_get_settings();
    RECOV_SERVO_move(SETI_get_settings()->recovery_open_angle);
    TIMER_start(IGNITER_TIMER_ON, settings->recovery_safety_trigger, TIMER_ONE_SHOT,
              TIMER_CB_igniter_high, NULL);
    TIMER_start(IGNITER_TIMER_OFF,
              settings->recovery_safety_trigger + settings->recovery_open_time,
              TIMER_ONE_SHOT, TIMER_CB_igniter_low, NULL);
    TIMER_start(BRAKE_TIMER, 3000, TIMER_ONE_SHOT, TIMER_CB_brake_close, NULL);
    TIMER_start(RECOV_SERVO_TIMER, 2000, TIMER_ONE_SHOT, TIMER_CB_recov_close, NULL);

    BUZZER_set_level(1);
    TIMER_stop_and_delete(BUZZER_TIMER);
    TIMER_start(BUZZER_TIMER, 1000, TIMER_ONE_SHOT, TIMER_CB_buzzer_low, NULL);
}

static void brake_event(void *h_arg, esp_event_base_t, int32_t id, void *data) {
    ESP_LOGD(TAG, "Brake event");
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

static void flash_keep_previous_data_on_queue(rocket_data_t *main_data) {
  if (uxQueueMessagesWaiting(rtos.data_to_flash) > RTOS_FLASH_QUEUE_DATA_TO_STASH) {
      rocket_data_t data;
      if (xQueueReceive(rtos.data_to_flash, (void*)&data, 0) != pdTRUE) {
        ESP_LOGE(TAG, "Unable to send to back");
      }
    }

    if (xQueueSend(rtos.data_to_flash, (void *)main_data, 0) == pdFALSE) {
        ESP_LOGE(TAG, "UNABLE TO SEND DATA TO FLASH TASK ON LAUNCHPAD");
    }
}

static void sensors_new_data_event(void *h_arg, esp_event_base_t, int32_t id,
                                   void *data) {
  // ESP_LOGI(TAG, "NEW DATA EVENT");
  main_data.sensors = *(sensors_t*)data;
  update_data();

  if (main_data.state < GROUND) {
    if (xQueueSend(rtos.data_to_memory, (void *)&main_data, 0) == pdFALSE) {
      ESP_LOGE(TAG, "UNABLE TO SEND DATA TO MEMORY TASK");
    }
  }

  if (main_data.state > LAUNCHPAD && main_data.state < GROUND) {
    if (xQueueSend(rtos.data_to_flash, (void *)&main_data, 0) == pdFALSE) {
      ESP_LOGE(TAG, "UNABLE TO SEND DATA TO FLASH TASK");
    }
  } else if (main_data.state == LAUNCHPAD) {
    flash_keep_previous_data_on_queue(&main_data);
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
