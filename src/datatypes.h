//
// Created by Aubrey McLeod on 2020-12-15.
//

#ifndef MAKEV20_DATATYPES_H
#define MAKEV20_DATATYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "global_settings.h"


/* used to track the number of times a given flag occurs; used for validation */
typedef struct cmd_flag_tracker_s{
    uint8_t input_flag;
    uint8_t output_flag;
    uint8_t overwrite_flag;
    uint8_t name_flag;
    uint8_t autoexec_flag;
    uint8_t video_flag;
    uint8_t help_flag;
    uint8_t version_flag;
    uint8_t debug_flag;

    uint8_t add_20_flag;
    uint8_t add_40_flag;
    uint8_t add_60_flag;
    uint8_t add_a0_flag;
    uint8_t add_b0_flag;
} cmd_flag_tracker_t;

/* command line indexes and settings. */
typedef struct cli_info_s{
    char *standard_version;
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

    /* flag tracking */
    cmd_flag_tracker_t flag_tracker;
} cli_info_t;

/* currently, the two types of flags are command flags (CMD_FLAG) and address flags (ADD_FLAG) */
typedef enum flag_type_s {
    CMD_FLAG,
    ADD_FLAG
} flag_type_t;

typedef struct cmd_flag_s{
    char *name;
    flag_type_t type;

    void (*set_func)(cli_info_t *info, int argc, char *argv[], int index, int arg_qty);

    bool REQUIRES_ARGS;

    const int max_args;
    const int min_args;

    /* The following parameters are used in the help */
    const char *param_name; /* The name of the parameters */
    const char *description; /* The description of the arguments */
}cmd_flag_t;



typedef struct rom_block_s{
    uint8_t byte[BLOCK_SIZE];
}rom_block_t;

typedef struct input_file_s{
    FILE *fp;
    uint8_t occupies;
    long length;
    int contained_blocks;
    int extra_bytes;
}input_file_t;


typedef struct file_info_s{
    int8_t standard_version;

    struct input_file_s input[ADDRESSABLE_LOCATIONS];
    FILE *output;

    uint8_t input_qty;

    uint8_t existant_blocks;
    rom_block_t blocks[TOTAL_BLOCKS];

    char *rom_name;
    char *auto_exec;

    /* flags */
    uint8_t video_mode;
}file_info_t;

typedef struct file_block_s{
    char *name;
    uint16_t length;

    void (*set_func)(file_info_t, uint16_t length);
} file_block_t;


typedef struct rom_standard_s{
    char *name;
    uint8_t version_number;
    file_block_t *block_rules;
    uint16_t number_of_rules;
} rom_standard_t;

#endif //MAKEV20_DATATYPES_H
