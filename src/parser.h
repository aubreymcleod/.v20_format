//
// Created by Aubrey McLeod on 2020-12-19.
//

#ifndef MAKEV20_PARSER_H
#define MAKEV20_PARSER_H

#include "datatypes.h"


#define PARSER_ERROR_HEADER "Parser Exception:"

int init_parser();
int register_arguments(const cmd_flag_t *flag_definitions);
int close_parser();

cli_info_t parse(int argc, char *argv[]);
cmd_flag_t lookup(char* given_name, int type);
int get_arg_type(char *argv[], int index);

#endif //MAKEV20_PARSER_H