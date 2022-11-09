// Copyright 2022 PWrInSpace

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>


typedef enum {
  SM_ZERO = 0,  // DO NOT USE
  LAUNCHPAD,
  FLIGHT,
  AIR_BRAKE_ON,
  APOGEE_CONFIRMED,
  ON_GROUND,
  SM_MAX  // DO NOT USE
} States;

typedef enum {
  SM_OK,
  SM_MEMORY_ERROR,
  SM_UNUSED_STATE,
  SM_STATE_CHANGE_ERROR,
  SM_NO_MORE_STATES,
  SM_INVALID_STATE,
  SM_RUN_ERROR,
  SM_NULL_FUNCTION,
} SM_Response;

typedef void (*state_change_callback)(void *arg);
typedef void (*end_looped_function)(void);

typedef struct {
  States state_name;
  state_change_callback callback;
  void *arg;
} state_t;

void SM_init();
SM_Response SM_add_state(States state_name,  state_change_callback callback, void* arg);
SM_Response SM_set_end_function(end_looped_function fct, uint32_t freq_ms);
States SM_get_current_state(void);
SM_Response SM_change_state(States new_state);
SM_Response SM_change_state_ISR(States new_state);
SM_Response SM_run(void);




#endif

