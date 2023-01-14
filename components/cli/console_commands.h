// Copyright 2022 PWrInSpace

#ifndef CONSOLE_COMMANDS_H
#define CONSOLE_COMMANDS_H


int CLI_state_machine_get_state(int argc, char **argv);
int CLI_reset_device(int argc, char **argv);
int CLI_reset_reason(int argc, char **argv);
int CLI_brake_move(int argc, char **argv);
int CLI_recov_move(int argc, char **argv);
int CLI_turn_on_test_mode(int argc, char** argv);



#endif
