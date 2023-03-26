// Copyright 2022 PWrInSpace

#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H


int CLI_state_machine_get_state(int argc, char **argv);
int CLI_reset_device(int argc, char **argv);
int CLI_reset_reason(int argc, char **argv);
int CLI_brake_move(int argc, char **argv);
int CLI_recov_move(int argc, char **argv);
int CLI_change_state(int argc, char **argv);

int CLI_set_brake_open_angle(int argc, char **argv);
int CLI_set_brake_close_angle(int argc, char **argv);
int CLI_set_recovery_open_angle(int argc, char **argv);
int CLI_set_recovery_close_angle(int argc, char **argv);
int CLI_set_alpha(int argc, char **argv);
int CLI_set_beta(int argc, char **argv);
int CLI_turn_on_test_mode(int argc, char **argv);
int CLI_buzzer_active(int argc, char **argv);
int CLI_print_settings(int argc, char **argv);

int CLI_set_brake_open_time(int argc, char **argv);
int CLI_set_safety_trigger_time(int argc, char **argv);
int CLI_set_recovery_open_time(int argc, char **argv);
int CLI_flash_read(int argc, char **argv);
int CLI_flash_format(int argcc, char **argv);

#endif
