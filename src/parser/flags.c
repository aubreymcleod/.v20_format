//
// Created by Aubrey McLeod on 2020-12-20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flags.h"
#include "../parser.h"

void parser_flag_version_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_help_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_debug_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_input_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_output_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_overwrite_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_name_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_autoexec_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_video_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);
void parser_flag_address_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);

const cmd_flag_t flag_definition[] = {
        /* version cmd flag */{
            .name = "version",
            .type = CMD_FLAG,
            .set_func =  parser_flag_version_handler,

            .REQUIRES_ARGS = false,
            .max_args = 0,
            .min_args = 0,
            .param_name = "Version Flag",
            .description = "Displays the current version number of this utility and the supported .v20 standards."},

        /* help cmd flag */{
            .name = "help",
            .type = CMD_FLAG,
            .set_func =  parser_flag_help_handler,

            .REQUIRES_ARGS = false,
            .max_args = 0,
            .min_args = 0,
            .param_name = "Help Flag",
            .description = "Displays a list of all registered commands and their associated descriptions/arguments."},

        /* debug cmd flag */{
            .name = "db",
            .type = CMD_FLAG,
            .set_func =  parser_flag_debug_handler,

            .REQUIRES_ARGS = false,
            .max_args = 0,
            .min_args = 0,
            .param_name = "Debug Flag",
            .description = "Displays a readout with the Parser's interpretation of the given ARG structure."},

        /* input cmd flag */{
            .name = "i",
            .type = CMD_FLAG,
            .set_func = parser_flag_input_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 10,
            .param_name = "Input Flag",
            .description = "Declares input files which need to be converted; takes optional address flags."},

        /* output cmd flag */{
            .name = "o",
            .type = CMD_FLAG,
            .set_func = parser_flag_output_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Output Flag",
            .description = "Declares output file."},

        /* overwrite cmd flag */{
            .name = "ow",
            .type = CMD_FLAG,
            .set_func = parser_flag_overwrite_handler,

            .REQUIRES_ARGS = false,
            .min_args = 0,
            .max_args = 0,
            .param_name = "Overwrite Flag",
            .description = "Sets the output to overwrite existing files."},

        /* name cmd flag */{
            .name = "n",
            .type = CMD_FLAG,
            .set_func = parser_flag_name_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Name Flag",
            .description = "Sets the name attribute for a given ROM."},

        /* autoexec cmd flag */{
            .name = "ae",
            .type = CMD_FLAG,
            .set_func = parser_flag_autoexec_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Auto-Exec Flag",
            .description = "Sets the Auto-Exec command for a given ROM."},

        /* video cmd flag */{
            .name = "v",
            .type = CMD_FLAG,
            .set_func = parser_flag_video_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Video Flag",
            .description = "Sets the video standard for a given ROM."},

        /* $2000 address flag */{
            .name = "20",
            .type = ADD_FLAG,
            .set_func = parser_flag_address_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Address $2000 Flag",
            .description = "Defines a ROM file which occupies $2000"},

        /* $4000 address flag */{
            .name = "40",
            .type = ADD_FLAG,
            .set_func = parser_flag_address_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Address $4000 Flag",
            .description = "Defines a ROM file which occupies $4000"},

        /* $6000 address flag */{
            .name = "60",
            .type = ADD_FLAG,
            .set_func = parser_flag_address_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Address $6000 Flag",
            .description = "Defines a ROM file which occupies $6000"},

        /* $A000 address flag */{
            .name = "a0",
            .type = ADD_FLAG,
            .set_func = parser_flag_address_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Address $A000 Flag",
            .description = "Defines a ROM file which occupies $A000"},

        /* $B000 address flag */{
            .name = "b0",
            .type = ADD_FLAG,
            .set_func = parser_flag_address_handler,

            .REQUIRES_ARGS = true,
            .min_args = 1,
            .max_args = 1,
            .param_name = "Address $B000 Flag",
            .description = "Defines a ROM file which occupies $B000"}
};


void parser_flag_version_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    info->version_flag = 1;
    info->flag_tracker.version_flag++;
}

void parser_flag_help_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    info->help_flag = 1;
    info->flag_tracker.help_flag++;
}

void parser_flag_debug_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    info->debug_mode = 1;
    info->flag_tracker.debug_flag++;
}

void parser_flag_input_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    int type;
    for(int i = index+1; i <= index+arg_qty; i++){
        type = get_arg_type(argv, i);
        /* if we hit an argument*/
        if(type == 0){
            info->unknown_inputs[info->unknown_input_qty] = i;
            info->unknown_input_qty++;
            info->total_input_qty++;
        }
        /* if we hit an address flag */
        else if(type == 2){
            cmd_flag_t arg = lookup(argv[i]+type, type);
            (*arg.set_func)(info, argc, argv, i, 1);
            i++;
        }
        /* If we hit something else, like a new command, when we really shouldn't */
        else{
            printf("%s parsing fault: \"%s\" was not expected as an argument for \"-i\".", PARSER_ERROR_HEADER, argv[index]);
            exit(109);
        }
    }

    info->flag_tracker.input_flag++;
}

void parser_flag_output_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    if(arg_qty != 1){
        printf("%s Output flag requires exactly one argument.", PARSER_ERROR_HEADER);
        exit(101);
    }

    info->output_index = index+1;
    info->flag_tracker.output_flag++;
}

void parser_flag_overwrite_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    info->overwrite_flag = 1;
    info->flag_tracker.overwrite_flag++;
}

void parser_flag_name_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    if(arg_qty != 1){
        printf("%s Name flag requires exactly one argument.", PARSER_ERROR_HEADER);
        exit(102);
    }

    info->name_index = index+1;
    info->flag_tracker.name_flag++;
}

void parser_flag_autoexec_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    if(arg_qty != 1){
        printf("%s Autoexec flag requires exactly one argument.", PARSER_ERROR_HEADER);
        exit(103);
    }

    info->autoexec_index = index+1;
    info->flag_tracker.autoexec_flag++;
}

void parser_flag_video_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    if(arg_qty != 1){
        printf("%s video flag requires exactly one argument.", PARSER_ERROR_HEADER);
        exit(104);
    }

    if(strcmp(argv[index+1], "ntsc") == 0){
        info->video_mode = 0;
    }
    else if(strcmp(argv[index+1], "pal") == 0){
        info->video_mode = 1;
    }
    else{
        printf("%s video flag can only be \"pal\" or \"ntsc\".", PARSER_ERROR_HEADER);
        exit(105);
    }

    info->flag_tracker.video_flag++;
}

void parser_flag_address_handler(cli_info_t *info, int argc, char *argv[], int index, int arg_qty){
    if(index+1 >= argc){
        printf("%s Address flag \"%s\" requires an argument.", PARSER_ERROR_HEADER, argv[index]);
        exit(106);
    }
    else if(get_arg_type(argv, index+1) != 0){
        printf("%s Address flag \"%s\" has an invalid argument, \"%s\" is a flag not an argument.", PARSER_ERROR_HEADER, argv[index], argv[index+1]);
        exit(107);
    }

    /* select memory block $2000 */
    if(strcmp(argv[index]+2, "20") == 0){
        info->index_20 = index+1;
        info->flag_tracker.add_20_flag++;
    }
    /* select memory block $4000 */
    else if(strcmp(argv[index]+2, "40") == 0){
        info->index_40 = index+1;
        info->flag_tracker.add_40_flag++;
    }
    /* select memory block $6000 */
    else if(strcmp(argv[index]+2, "60") == 0){
        info->index_60 = index+1;
        info->flag_tracker.add_60_flag++;
    }
    /* select memory block $A000 */
    else if(strcmp(argv[index]+2, "a0") == 0){
        info->index_a0 = index+1;
        info->flag_tracker.add_a0_flag++;
    }
    /* select memory block $B000 */
    else if(strcmp(argv[index]+2, "b0") == 0){
        info->index_b0 = index+1;
        info->flag_tracker.add_b0_flag++;
    }
    /* Unknown memory block encountered */
    else{
        printf("%s Address flag \"%s\" does not correspond to a known memory block.", PARSER_ERROR_HEADER, argv[index]);
        exit(108);
    }

    info->total_input_qty++;
}