//
// Created by Aubrey McLeod on 2020-12-19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"



static cmd_flag_t *cli_arguments;
static unsigned int num_arguments;

/* begin a new parser session */
int init_parser(){
    free(cli_arguments);
    cli_arguments = NULL;
    num_arguments = 0;

    cli_arguments = (cmd_flag_t*)calloc(NUM_ALLOCATED_ARGUMENTS, sizeof(cmd_flag_t));
    return 0;
}

/* load our command flag definitions */
int register_arguments(const cmd_flag_t *flag_definitions){
    for(int i = 0; i<NUM_ALLOCATED_ARGUMENTS; i++){
        memcpy(&cli_arguments[i], &flag_definitions[i], sizeof(cmd_flag_t));
        num_arguments++;
    }
    //register the list of arguments to the parser
    return 0;
}

/* clear out our parser arguments */
int close_parser(){
    free(cli_arguments);
    return 0;
}

/* return a corresponding command type based on the command name */
cmd_flag_t lookup(char* given_name, int type){
    flag_type_t t = (type == 1) ? CMD_FLAG : ADD_FLAG;
    for(int i = 0; i < num_arguments; i++){
        if(strcmp(given_name, cli_arguments[i].name) == 0 && t == cli_arguments[i].type){
            return cli_arguments[i];
        }
    }

    /* This state will only be reached if we found no matching flag */
    printf("%s could not identify %s flag: %s.", PARSER_ERROR_HEADER, (t==CMD_FLAG) ? "command" : "Address", given_name);
    exit(5);
}

/* returns a value based on what type of argument is passed
 * 0 for argument, 1 for cmd flag, 2 for an address flag
 * causes process termination on invalid flag.
 *
 * Takes the arg array string, and the index;
 * index is used for error messages */
int get_arg_type(char *argv[], int index){
    /* Test if flag */
    if(argv[index][0] == '-') {
        /* if we hit an empty flag, exit raise an exception */
        if (argv[index][1] == '\0') {
            printf("%s command flag at index %d is incomplete.\n", PARSER_ERROR_HEADER, index);
            exit(1);
        }
        /* if we hit an address flag, confirm that it is not empty */
        else if (argv[index][1] == '-') {
            if (argv[index][2] == '\0') {
                printf("%s address flag at index %d is incomplete\n", PARSER_ERROR_HEADER, index);
                exit(2);
            }
            return 2;
        }
        /* if we don't hit a CMD flag */
        else if (argv[index][1] != '-') {
            return 1;
        }
    }
    /* if we don't hit a flag, then we hit an arg. */
    return 0;
}


/* returns the number of arguments between command flags, starting at the point after a given index
 * and ending at the point before either the end of the argv[] array or the next command flag */
int get_args_qty(int argc, char *argv[], int initial_index){
    int index = initial_index+1;
    int arg_type;
    while(index < argc){
        arg_type = get_arg_type(argv, index);
        /* if we run into a command flag */
        if(arg_type == 1){
            break;
        }
        index++;
    }
    return index - initial_index - 1;
}

/* parse information from the command line arguments, store them in a CLI_info type and then return the CLI_info structure */
cli_info_t parse(int argc, char *argv[]){
    cli_info_t info = {0};
    int arg_type;
    for(int i = 1; i < argc && argv[i] !=  NULL; i++){
        arg_type = get_arg_type(argv, i);

        /* if we have a command flag */
        if(arg_type == 1){
            int arg_qty = 0;
            cmd_flag_t arg = lookup(argv[i]+arg_type, arg_type);
            if(arg.REQUIRES_ARGS){
                arg_qty = get_args_qty(argc, argv, i);
                if(arg_qty > arg.max_args){
                    printf("%s Too many arguments for command \"%s\", received %d (max of %d)", PARSER_ERROR_HEADER, argv[i], arg_qty, arg.max_args);
                    exit(6);
                }
                else if(arg_qty < arg.min_args){
                    printf("%s Too few arguments for command \"%s\", received %d (min of %d)", PARSER_ERROR_HEADER, argv[i], arg_qty, arg.min_args);
                    exit(7);
                }
            }

            (*arg.set_func)(&info, argc, argv, i, arg_qty);
            i += arg_qty;
        }

        /* if we have a address flag */
        else if(arg_type == 2){
            printf("%s isolated address flag %s at index %d.", PARSER_ERROR_HEADER, argv[i], i);
            exit(3);
        }

        /* if we hit an argument */
        else{
            printf("%s isolated argument %s at index %d.", PARSER_ERROR_HEADER, argv[i], i);
            exit(4);
        }
    }
    info.standard_version = ".v20-1";
    return info;
}
