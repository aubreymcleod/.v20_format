//
// Created by Aubrey McLeod on 2020-12-20.
//

#include <stdio.h>
#include <stdlib.h>
#include "parser_validation.h"

void validate_convert_mode(cli_info_t *info){
    uint16_t convert_flags_sum;
    /* Load all the values to see if we are in convert mode (sum > 0)*/
    convert_flags_sum += info->flag_tracker.output_flag;
    convert_flags_sum += info->flag_tracker.input_flag;
    convert_flags_sum += info->flag_tracker.overwrite_flag;
    convert_flags_sum += info->flag_tracker.name_flag;
    convert_flags_sum += info->flag_tracker.autoexec_flag;
    convert_flags_sum += info->flag_tracker.video_flag;
    convert_flags_sum += info->flag_tracker.add_20_flag;
    convert_flags_sum += info->flag_tracker.add_40_flag;
    convert_flags_sum += info->flag_tracker.add_60_flag;
    convert_flags_sum += info->flag_tracker.add_a0_flag;
    convert_flags_sum += info->flag_tracker.add_b0_flag;

    if(convert_flags_sum > 0){ /* we are indeed attempting to run a conversion operation.*/
        if(info->flag_tracker.output_flag == 0 || info->flag_tracker.input_flag == 0){
            printf("%s you appear to be trying to run a conversion operation without having both an input (\"-i\") and/or an output (\"-o\") specified", PARSER_ERROR_HEADER);
            exit(120);
        }
        else if(info->flag_tracker.output_flag > 1){
            printf("%s you have specified multiple output files, please specify only one.", PARSER_ERROR_HEADER);
            exit(121);
        }
        else if(info->flag_tracker.overwrite_flag > 1){
            printf("%s you have specified multiple overwrite flags, please specify only one.", PARSER_ERROR_HEADER);
            exit(122);
        }
        else if(info->flag_tracker.name_flag > 1){
            printf("%s you have specified multiple name flags, please specify only one.", PARSER_ERROR_HEADER);
            exit(123);
        }
        else if(info->flag_tracker.autoexec_flag > 1){
            printf("%s you have specified multiple auto-exec flags, please specify only one.", PARSER_ERROR_HEADER);
            exit(124);
        }
        else if(info->flag_tracker.video_flag > 1){
            printf("%s you have specified multiple video flags, please specify only one.", PARSER_ERROR_HEADER);
            exit(125);
        }
        else if(info->flag_tracker.add_20_flag > 1){
            printf("%s you are attempting to address $2000 with multiple files.", PARSER_ERROR_HEADER);
            exit(126);
        }
        else if(info->flag_tracker.add_40_flag > 1){
            printf("%s you are attempting to address $4000 with multiple files.", PARSER_ERROR_HEADER);
            exit(127);
        }
        else if(info->flag_tracker.add_60_flag > 1){
            printf("%s you are attempting to address $6000 with multiple files.", PARSER_ERROR_HEADER);
            exit(128);
        }
        else if(info->flag_tracker.add_a0_flag > 1){
            printf("%s you are attempting to address $A000 with multiple files.", PARSER_ERROR_HEADER);
            exit(129);
        }
        else if(info->flag_tracker.add_b0_flag > 1){
            printf("%s you are attempting to address $B000 with multiple files.", PARSER_ERROR_HEADER);
            exit(130);
        }

        /* charcteristics of input flag */
        else if(info->total_input_qty > ADDRESSABLE_LOCATIONS){
            printf("%s you provided %d input files where there are only %d addressable locations.", PARSER_ERROR_HEADER, info->total_input_qty, ADDRESSABLE_LOCATIONS);
            exit(131);
        }
    }
}


void validate_help_cmd(cli_info_t *info){
    if(info->flag_tracker.help_flag > 1){
        printf("%s you used the help flag multiple times, only once is fine.", PARSER_ERROR_HEADER);
        exit(140);
    }
}

void validate_version_cmd(cli_info_t *info){
    if(info->flag_tracker.version_flag > 1){
        printf("%s you used the version flag multiple times, only once is fine.", PARSER_ERROR_HEADER);
        exit(141);
    }
}

void validate_debug_cmd(cli_info_t *info){
    if(info->flag_tracker.debug_flag > 1){
        printf("%s you used the debug flag multiple times, only once is fine.", PARSER_ERROR_HEADER);
        exit(142);
    }
}

void validate_cli(cli_info_t info){
    validate_convert_mode(&info);
    validate_help_cmd(&info);
    validate_version_cmd(&info);
    validate_debug_cmd(&info);
}