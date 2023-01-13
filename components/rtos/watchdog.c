#include "watchdog.h"

static struct m_watchdog {
  TaskHandle_t feeders[WATCHDOG_MAX_TASKS];
  bool flags[WATCHDOG_MAX_TASKS];
  size_t delay;
  TaskHandle_t watchdog_task_handle;
  watchdog_callback_t user_callback;
  size_t feeder_count;
} g_watchdog;  // the global watchdog

void watchdog_background_task(void* arg) {
  while (true) {
    watchdog_check_all();
    vTaskDelay(pdMS_TO_TICKS(g_watchdog.delay));
  }
}

void watchdog_init(size_t delay, size_t watchdog_task_stack_size,
                   size_t watchdog_task_priority,
                   watchdog_callback_t callback) {
  g_watchdog.user_callback = callback;
  g_watchdog.feeder_count = 0;
  g_watchdog.delay = pdMS_TO_TICKS(delay);
  for (int i = WATCHDOG_MAX_TASKS - 1; i >= 0; i--) {
    g_watchdog.feeders[i] = 0;
  }
  xTaskCreate((void*)watchdog_background_task, 0, watchdog_task_stack_size, 0,
              watchdog_task_priority, g_watchdog.watchdog_task_handle);
}
void watchdog_disconnect() {
  vTaskDelete(g_watchdog.watchdog_task_handle);
  g_watchdog.watchdog_task_handle = 0;
}

void watchdog_reconnect(size_t delay, size_t watchdog_task_stack_size,
                        size_t watchdog_task_priority,
                        watchdog_callback_t callback) {
  if (g_watchdog.watchdog_task_handle != NULL) {
    vTaskDelete(g_watchdog.watchdog_task_handle);
  }
  xTaskCreate((void*)watchdog_background_task, 0, watchdog_task_stack_size, 0,
              watchdog_task_priority, g_watchdog.watchdog_task_handle);
}

bool watchdog_append(TaskHandle_t feeder) {
  if (g_watchdog.feeder_count == WATCHDOG_MAX_TASKS) {
    return false;
  }
  int i = 0;
  while (g_watchdog.feeders[i]) {
    i++;
  }
  g_watchdog.feeders[i] = feeder;
  g_watchdog.flags[i] = false;
  g_watchdog.feeder_count++;
  return true;
}

bool watchdog_pop(TaskHandle_t feeder) {
  int i = 0;
  while (i < WATCHDOG_MAX_TASKS && (g_watchdog.feeders[i] != feeder)) {
    i++;
  }
  if (i == WATCHDOG_MAX_TASKS) {
    return false;
  }
  g_watchdog.feeders[i] = 0;
  return true;
}

int watchdog_feed(TaskHandle_t feeder) {
  int i = 0;
  while (i < WATCHDOG_MAX_TASKS && g_watchdog.feeders[i] != feeder) {
    i++;
  }
  if (i == WATCHDOG_MAX_TASKS) {
    return 1;
  }
  g_watchdog.flags[i] = false;
  return 0;
}
int watchdog_check(TaskHandle_t feeder) {
  int i = 0;
  while (i < WATCHDOG_MAX_TASKS && g_watchdog.feeders[i] != feeder) {
    i++;
  }
  if (i == WATCHDOG_MAX_TASKS) {
    return 1;
  }
  if ((g_watchdog.feeders[i]) && g_watchdog.flags[i]) {
    watchdog_starve(feeder);
  }
  g_watchdog.flags[i] = true;
  return 0;
}

int watchdog_check_all() {
  for (int i = 0; i < WATCHDOG_MAX_TASKS; i++) {
    if ((g_watchdog.feeders[i]) && g_watchdog.flags[i]) {
      watchdog_starve(g_watchdog.feeders[i]);
    }
    g_watchdog.flags[i] = true;
  }
  return 0;
}
int watchdog_starve(TaskHandle_t feeder) {
  g_watchdog.user_callback(feeder);
  return 0;
}
