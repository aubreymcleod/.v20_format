//
// Created by Aubrey McLeod on 2020-12-15.
//

#ifndef MAKEV20_DATATYPES_H
#define MAKEV20_DATATYPES_H

#include <stdint.h>
#include "global_settings.h"

typedef struct cli_info{
    uint8_t unknown_inputs[ADDRESSABLE_LOCATIONS];
    uint8_t unknown_input_qty;
    uint8_t total_input_qty;
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

#endif //MAKEV20_DATATYPES_H
