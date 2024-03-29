// Copyright 2022 PWrInSpace

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

typedef enum {
  SM_OK,
  SM_STATES_ERROR,
  SM_STATES_NUMBER_ERROR,
  SM_STATE_CHANGE_ERROR,
  SM_NO_MORE_STATES,
  SM_INVALID_STATE,
  SM_RUN_ERROR,
  SM_NULL_FUNCTION,
} SM_Response;

typedef void (*state_change_callback)(void *arg);
typedef void (*end_looped_function)(void);
typedef uint8_t state_id;

typedef struct {
  state_id id;
  state_change_callback callback;
  void *arg;
} state_config_t;

void SM_init();
SM_Response SM_set_states(state_config_t *states, uint8_t states_number);
SM_Response SM_set_end_function(end_looped_function fct, uint32_t freq_ms);
uint8_t SM_get_current_state(void);
SM_Response SM_change_state(state_id new_state);
SM_Response SM_change_state_ISR(state_id new_state);
SM_Response SM_run(void);
SM_Response SM_destroy(void);




#endif

