//
// Created by Aubrey McLeod on 2020-12-09.
//

#include <stdio.h>
#include <stdint.h>

#include "generator.h"
#include "parser.h"
#include "parser/flags.h"
#include "parser/parser_validation.h"
#include "generator/standards.h"

void display_cli_info(cli_info_t, char *[]);

int main(int argc, char *argv[]){
    init_parser();
    register_arguments(flag_definition);
    cli_info_t info = parse(argc, argv);
    validate_cli(info);

    if(info.debug_mode) {
        display_cli_info(info, argv);
    }

    if(info.output_index) {
        printf("===---- Attempting to write .v20 formatted file... ----===\n");
        generate(info, argv);
        printf("Write Successful!\n\n");
    }

    if(info.help_flag) {
        printf("========================== Help ===========================\n");
        printf("the following flags are used for control with this utility\n");
        printf("===========================================================\n");
        for(uint16_t i = 0; i < NUM_ALLOCATED_ARGUMENTS; i++){
            printf("%s:\n\"%s%s\"\n%s\n\n", flag_definition[i].param_name, (flag_definition[i].type == CMD_FLAG) ? "-" : "--", flag_definition[i].name,flag_definition[i].description);
        }
    }

    if(info.version_flag) {
        printf("========================= Version =========================\n");
        printf("Current makev20 version: %.1f\n", VERSION_NUM);
        printf("Supported standards file standards: ");
        for(uint16_t i = 0; i<supported_standards; i++){
            printf("%d, ",standards_definitions[i].version_number);
        }
        printf("\n\n");
    }

    close_parser();
    return 0;
}


//debug function
void display_cli_info(cli_info_t info, char *argv[]){
    printf("========================= PARSER =========================\n");
    printf("===---INPUTS     ---===\n");
    printf("Identified inputs: %d\n", info.total_input_qty);
    printf("Generic inputs: ");
    for(uint8_t i=0; i<info.unknown_input_qty; i++)
        printf("%s, ", argv[info.unknown_inputs[i]]);
    printf("\nInputs at $2000: %s", (info.index_20)? argv[info.index_20] : "");
    printf("\nInputs at $4000: %s", (info.index_40)? argv[info.index_40] : "");
    printf("\nInputs at $6000: %s", (info.index_60)? argv[info.index_60] : "");
    printf("\nInputs at $A000: %s", (info.index_a0)? argv[info.index_a0] : "");
    printf("\nInputs at $B000: %s\n", (info.index_b0)? argv[info.index_b0] : "");

    printf("===---OUTPUTS    ---===\n");
    printf("Output: %s\n", (info.output_index)? argv[info.output_index] : "");

    printf("===---NAME       ---===\n");
    printf("Name: %s\n", (info.name_index)? argv[info.name_index] : "");

    printf("===---AUTOEXEC   ---===\n");
    printf("AutoExec: %s\n", (info.autoexec_index)? argv[info.autoexec_index] : "");

    printf("===---VIDEO      ---===\n");
    printf("Video Mode: %s\n", (info.video_mode)? "pal" : "ntsc");

    printf("===---CMD FLAGS  ---===\n");
    printf("Overwrite: %s\n", (info.overwrite_flag)? "1" : "0");
    printf("Version Request: %s\n", (info.version_flag)? "1" : "0");
    printf("Help Request: %s\n", (info.help_flag)? "1" : "0");

    printf("Debug Output: %s\n", (info.debug_mode)? "1" : "0");
    printf("=======================\n\n");
}