//
// Created by Aubrey McLeod on 2020-12-09.
//

#include <stdio.h>
#include <stdint.h>
#include "parser/argument_parser.h"
#include "generator/generate_rom.h"

void display_cli_info(cli_info, char *[]);

int main(int argc, char *argv[]){
    cli_info info = parse_input((uint8_t)argc, argv);
    if(info.debug_mode)
        display_cli_info(info, argv);

    if(info.output_index)
        make_rom(info, argv);

    if(info.help_flag)
        printf("Write help menu");

    if(info.version_flag)
        printf("Write version info");


    return 0;
}


//debug function
void display_cli_info(cli_info info, char *argv[]){
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

    printf("===---NAME       ---===\n");
    printf("Name: %s\n", (info.name_index)? argv[info.name_index] : "");

    printf("===---VIDEO      ---===\n");
    printf("Video Mode: %s\n", (info.video_mode)? "pal" : "ntsc");

    printf("===---CMD FLAGS  ---===\n");
    printf("Overwrite: %s\n", (info.overwrite_flag)? "1" : "0");
    printf("Version Request: %s\n", (info.version_flag)? "1" : "0");
    printf("Help Request: %s\n", (info.help_flag)? "1" : "0");

    printf("Debug Output: %s\n", (info.debug_mode)? "1" : "0");
}