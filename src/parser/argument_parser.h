//
// Created by Aubrey McLeod on 2020-12-10.
//

#ifndef V20_GENERATOR_ARGUMENT_PARSER_H
#define V20_GENERATOR_ARGUMENT_PARSER_H

#define ADDRESSABLE_LOCATIONS 5

#include <stdint.h>

typedef struct cli_info{
    uint8_t unknown_inputs[ADDRESSABLE_LOCATIONS];
    uint8_t unknown_input_qty;
    uint8_t output_index;
    uint8_t name_index;
    uint8_t autoexec_index;
    uint8_t index_20;
    uint8_t index_40;
    uint8_t index_60;
    uint8_t index_a0;
    uint8_t index_b0;
    uint8_t overwrite_flag: 1;
    uint8_t version_flag: 1;
    uint8_t help_flag: 1;
    uint8_t video_mode: 1;

    uint8_t debug_mode: 1;
}cli_info;

cli_info parse_input(uint8_t, char*[]);

#endif //V20_GENERATOR_ARGUMENT_PARSER_H
