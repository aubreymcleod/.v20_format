//
// Created by Aubrey McLeod on 2020-12-10.
//

//General Execution
#define VERSION_FLAG "-version" //[-version]
#define VERSION_FLAG_ID 70

#define HELP_FLAG "-help"       //[-help]
#define HELP_FLAG_ID 80

#define DEBUG_FLAG "-db"        //[-db]
#define DEBUG_FLAG_ID 130

//Conversion Flags
#define INPUT_FLAG "-i"         //-i [mem_flag] <path_to_file0> [[mem_flag] <path_to_file1> ...]
#define INPUT_FLAG_ID 10
#define INPUT_ARG_ID 201

#define OUTPUT_FLAG "-o"        //-o <path_to_file>
#define OUTPUT_FLAG_ID 20
#define OUTPUT_ARG_ID 202

#define OVERWRITE_FLAG "-ow"    //[-ow]
#define OVERWRITE_FLAG_ID 30

#define NAME_FLAG "-n"          //[-n "<NAME>"]
#define NAME_FLAG_ID 40
#define NAME_ARG_ID 203

#define AUTOEXEC_FLAG "-ae"     //[-ae "<CMD>"]
#define AUTOEXEC_FLAG_ID 50
#define AUTOEXEC_ARG_ID 204

#define VIDEO_TYPE_FLAG "-v"    //[-v <ntsc/pal>]
#define VIDEO_TYPE_FLAG_ID 60
#define VIDEO_TYPE_ARG_ID 205

#define VIDEO_TYPE_NTSC "ntsc"
#define VIDEO_TYPE_NTSC_ID 190
#define VIDEO_TYPE_PAL  "pal"
#define VIDEO_TYPE_PAL_ID 191



//Addressing Flags
#define MEMORY_A000_FLAG "--a0" //[--a0 <path_to_file>]
#define MEMORY_A000_FLAG_ID 210
#define MEMORY_A000_ARG_ID 211

#define MEMORY_B000_FLAG "--b0" //[--b0 <path_to_file>]
#define MEMORY_B000_FLAG_ID 220
#define MEMORY_B000_ARG_ID 221

#define MEMORY_2000_FLAG "--20" //[--20 <path_to_file>]
#define MEMORY_2000_FLAG_ID 230
#define MEMORY_2000_ARG_ID 231

#define MEMORY_4000_FLAG "--40" //[--40 <path_to_file>]
#define MEMORY_4000_FLAG_ID 240
#define MEMORY_4000_ARG_ID 241

#define MEMORY_6000_FLAG "--60" //[--60 <path_to_file>]
#define MEMORY_6000_FLAG_ID 250
#define MEMORY_6000_ARG_ID 251

#define UNKNOWN_ARG_ID 255

/* FLAGS and their respective identities
 * -i       ->      10      Generic Arg = 201
 * -o       ->      20      Arg         = 202
 * -ow      ->      30
 * -n       ->      40      Arg         = 203
 * -ae      ->      50      Arg         = 204
 * -v       ->      60      Arg         = 205   -> ntsc = 190   pal = 191
 * -version ->      70
 * -help    ->      80
 *
 * --a0     ->      210     Arg         = 211
 * --b0     ->      220     Arg         = 221
 * --20     ->      230     Arg         = 231
 * --40     ->      240     Arg         = 241
 * --60     ->      250     Arg         = 251
 * */

#include "argument_parser.h"
#include "parser_execeptions.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

uint8_t* arg_mask;


//argument masking
void mask_args(uint8_t, char*[]);
uint8_t infer_argument_id(uint8_t, uint8_t, uint8_t, uint8_t);

//validation
bool has_args(uint8_t);
bool is_cmd_flag(uint8_t);
void validate_arg_structure(uint8_t, char *argv[]);

//extrapolation
void populate_CLI_INFO(uint8_t, cli_info*);

//error handling
void raise_exception(int);



cli_info parse_input(uint8_t argc, char *argv[]){
    arg_mask = (uint8_t*)calloc(argc,sizeof(uint8_t));
    mask_args(argc, argv);

    cli_info Info = {0};
    validate_arg_structure(argc, argv);
    populate_CLI_INFO(argc, &Info);

    free(arg_mask);
    return Info;
}


void mask_args(uint8_t argc, char* argv[]){
    uint8_t last_cmd_flag = 0;
    uint8_t last_cmd_index = 0;
    uint8_t last_arg_id = 0;

    for(uint8_t i = 1; i<argc; i++){

        //MAIN COMMAND FLAGS
        if(!strcmp(argv[i], INPUT_FLAG)){
            arg_mask[i] = INPUT_FLAG_ID;
        }

        else if(!strcmp(argv[i], OUTPUT_FLAG)){
            arg_mask[i] = OUTPUT_FLAG_ID;
        }

        else if(!strcmp(argv[i], OVERWRITE_FLAG)){
            arg_mask[i] = OVERWRITE_FLAG_ID;
        }

        else if(!strcmp(argv[i], NAME_FLAG)){
            arg_mask[i] = NAME_FLAG_ID;
        }

        else if(!strcmp(argv[i], AUTOEXEC_FLAG)){
            arg_mask[i] = AUTOEXEC_FLAG_ID;
        }

        else if(!strcmp(argv[i], VIDEO_TYPE_FLAG)){
            arg_mask[i] = VIDEO_TYPE_FLAG_ID;
        }

        else if(!strcmp(argv[i], VERSION_FLAG)){
            arg_mask[i] = VERSION_FLAG_ID;
        }

        else if(!strcmp(argv[i], HELP_FLAG)){
            arg_mask[i] = HELP_FLAG_ID;
        }

        else if(!strcmp(argv[i], DEBUG_FLAG)){
            arg_mask[i] = DEBUG_FLAG_ID;
        }

        //MEM FLAGS AND ARGS INDEXING
        if(arg_mask[i] != 0){
            last_cmd_flag = arg_mask[i];
            last_cmd_index = i;
        }

        else if(!strcmp(argv[i], MEMORY_A000_FLAG)){
            arg_mask[i] = MEMORY_A000_FLAG_ID;
        }

        else if(!strcmp(argv[i], MEMORY_B000_FLAG)){
            arg_mask[i] = MEMORY_B000_FLAG_ID;
        }

        else if(!strcmp(argv[i], MEMORY_2000_FLAG)){
            arg_mask[i] = MEMORY_2000_FLAG_ID;
        }

        else if(!strcmp(argv[i], MEMORY_4000_FLAG)){
            arg_mask[i] = MEMORY_4000_FLAG_ID;
        }

        else if(!strcmp(argv[i], MEMORY_6000_FLAG)){
            arg_mask[i] = MEMORY_6000_FLAG_ID;
        }

        else{
            arg_mask[i] = infer_argument_id(i, last_cmd_flag, last_cmd_index, last_arg_id);
        }
        last_arg_id = arg_mask[i];
    }
}


uint8_t infer_argument_id(uint8_t current_index, uint8_t previous_cmd, uint8_t previous_cmd_index, uint8_t previous_arg_id){
    if(previous_cmd == INPUT_FLAG_ID){
        switch(previous_arg_id){
            case MEMORY_A000_FLAG_ID:
                return MEMORY_A000_ARG_ID;
            case MEMORY_B000_FLAG_ID:
                return MEMORY_B000_ARG_ID;
            case MEMORY_2000_FLAG_ID:
                return MEMORY_2000_ARG_ID;
            case MEMORY_4000_FLAG_ID:
                return MEMORY_4000_ARG_ID;
            case MEMORY_6000_FLAG_ID:
                return MEMORY_6000_ARG_ID;
            default:
                return INPUT_ARG_ID;
        }
    }

    else if(previous_cmd == OUTPUT_FLAG_ID && previous_cmd_index == current_index-1){
        return OUTPUT_ARG_ID;
    }

    else if(previous_cmd == NAME_FLAG_ID && previous_cmd_index == current_index-1){
        return NAME_ARG_ID;
    }

    else if(previous_cmd == AUTOEXEC_FLAG_ID && previous_cmd_index == current_index-1){
        return AUTOEXEC_ARG_ID;
    }

    else if(previous_cmd == VIDEO_TYPE_FLAG_ID && previous_cmd_index == current_index-1){
        return VIDEO_TYPE_ARG_ID;
    }

    else{
        return UNKNOWN_ARG_ID;
    }
}


//TODO: make validate_arg_structure, which ensures that there are no hanging commands, that each command had exactly the
// right number of commands, and that there are know unknown args
void validate_arg_structure(uint8_t argc, char *argv[]){
    uint8_t last_cmd = 0;
    uint8_t last_arg = 0;

    uint8_t inputfiles = 0;
    uint8_t outputfiles = 0;

    uint8_t video_signals = 0;
    uint8_t names = 0;
    uint8_t autoexecs = 0;

    uint8_t mem20s = 0;
    uint8_t mem40s = 0;
    uint8_t mem60s = 0;
    uint8_t memA0s = 0;
    uint8_t memB0s = 0;

    for(uint8_t i = 1; i<argc; i++){
        if(is_cmd_flag(arg_mask[i]) && has_args(last_arg))
            raise_exception(2);

        if(i==argc-1 && has_args(arg_mask[i]))
            raise_exception(2);

        if(is_cmd_flag(arg_mask[i]))
            last_cmd = arg_mask[i];

        switch(arg_mask[i]){
            case UNKNOWN_ARG_ID:
                raise_exception(0);
                break;
            case INPUT_ARG_ID:
                inputfiles++;
                if(inputfiles>ADDRESSABLE_LOCATIONS)
                    raise_exception(3);
                if(last_cmd != INPUT_FLAG_ID)
                    raise_exception(1);
                break;
            case OUTPUT_ARG_ID:
                outputfiles++;
                if(outputfiles>1)
                    raise_exception(4);
                if(last_cmd != OUTPUT_FLAG_ID)
                    raise_exception(1);
                break;
            case VIDEO_TYPE_ARG_ID:
                video_signals++;
                if(!strcmp(argv[i], VIDEO_TYPE_NTSC))
                    arg_mask[i] = VIDEO_TYPE_NTSC_ID;
                else if(!strcmp(argv[i], VIDEO_TYPE_PAL))
                    arg_mask[i] = VIDEO_TYPE_PAL_ID;

                if(video_signals>1)
                    raise_exception(7);
                if(last_cmd != VIDEO_TYPE_FLAG_ID)
                    raise_exception(1);
                break;
            case NAME_ARG_ID:
                names++;
                if(names>1)
                    raise_exception(5);
                if(last_cmd != NAME_FLAG_ID)
                    raise_exception(1);
                break;
            case AUTOEXEC_ARG_ID:
                autoexecs++;
                if(autoexecs>1)
                    raise_exception(6);
                if(last_cmd != AUTOEXEC_FLAG_ID)
                    raise_exception(1);
                break;
            case MEMORY_2000_ARG_ID:
                inputfiles++;
                mem20s++;
                if(inputfiles>ADDRESSABLE_LOCATIONS)
                    raise_exception(3);
                if(mem20s>1)
                    raise_exception(12);
                if(last_cmd != INPUT_FLAG_ID)
                    raise_exception(1);
                break;
            case MEMORY_4000_ARG_ID:
                inputfiles++;
                mem40s++;
                if(inputfiles>ADDRESSABLE_LOCATIONS)
                    raise_exception(3);
                if(mem40s>1)
                    raise_exception(13);
                if(last_cmd != INPUT_FLAG_ID)
                    raise_exception(1);
                break;
            case MEMORY_6000_ARG_ID:
                inputfiles++;
                mem60s++;
                if(inputfiles>ADDRESSABLE_LOCATIONS)
                    raise_exception(3);
                if(mem20s>1)
                    raise_exception(14);
                if(last_cmd != INPUT_FLAG_ID)
                    raise_exception(1);
                break;
            case MEMORY_A000_ARG_ID:
                inputfiles++;
                memA0s++;
                if(inputfiles>ADDRESSABLE_LOCATIONS)
                    raise_exception(3);
                if(memA0s>1)
                    raise_exception(15);
                if(last_cmd != INPUT_FLAG_ID)
                    raise_exception(1);
                break;
            case MEMORY_B000_ARG_ID:
                inputfiles++;
                memB0s++;
                if(inputfiles>ADDRESSABLE_LOCATIONS)
                    raise_exception(3);
                if(memB0s>1)
                    raise_exception(16);
                if(last_cmd != INPUT_FLAG_ID)
                    raise_exception(1);
                break;

        }
        last_arg = arg_mask[i];
    }
    //Node Pairings
    if(inputfiles>0 && outputfiles==0)
        raise_exception(30);
    else if(inputfiles==0 && outputfiles>0)
        raise_exception(31);
}
bool is_cmd_flag(uint8_t cmd_id){
    if(cmd_id == INPUT_FLAG_ID || cmd_id == OUTPUT_FLAG_ID || cmd_id == NAME_FLAG_ID || cmd_id == AUTOEXEC_FLAG_ID || cmd_id == VIDEO_TYPE_FLAG_ID || cmd_id == OVERWRITE_FLAG_ID || cmd_id == HELP_FLAG_ID || cmd_id == VERSION_FLAG_ID || cmd_id == DEBUG_FLAG_ID)
        return true;
    else
        return false;
}

bool has_args(uint8_t cmd_id){
    if(cmd_id == INPUT_FLAG_ID || cmd_id == OUTPUT_FLAG_ID || cmd_id == NAME_FLAG_ID || cmd_id == AUTOEXEC_FLAG_ID || cmd_id == VIDEO_TYPE_FLAG_ID || cmd_id == MEMORY_2000_FLAG_ID || cmd_id == MEMORY_4000_FLAG_ID || cmd_id == MEMORY_6000_FLAG_ID || cmd_id == MEMORY_A000_FLAG_ID || cmd_id == MEMORY_B000_FLAG_ID)
        return true;
    else
        return false;
}

//TODO: make populate_CLI_INFO which takes the mask and the values, and fills the ROM object accordingly.
void populate_CLI_INFO(uint8_t argc, cli_info *Info){
    for(int i=1; i<argc; i++){
        switch(arg_mask[i]){
            case MEMORY_2000_ARG_ID:
                Info->index_20=i;
                Info->total_input_qty++;
                break;
            case MEMORY_4000_ARG_ID:
                Info->index_40=i;
                Info->total_input_qty++;
                break;
            case MEMORY_6000_ARG_ID:
                Info->index_60=i;
                Info->total_input_qty++;
                break;
            case MEMORY_A000_ARG_ID:
                Info->index_a0=i;
                Info->total_input_qty++;
                break;
            case MEMORY_B000_ARG_ID:
                Info->index_b0=i;
                Info->total_input_qty++;
                break;

            case INPUT_ARG_ID:
                Info->unknown_inputs[Info->unknown_input_qty]=i;
                Info->unknown_input_qty++;
                Info->total_input_qty++;
                break;


            case OUTPUT_ARG_ID:
                Info->output_index=i;
                break;
            case NAME_ARG_ID:
                Info->name_index=i;
                break;
            case AUTOEXEC_ARG_ID:
                Info->autoexec_index=i;
                break;

            //video mode handling
            case VIDEO_TYPE_PAL_ID:
                Info->video_mode=1;
                break;
            case VIDEO_TYPE_NTSC_ID:
                Info->video_mode=0;
                break;
            case VIDEO_TYPE_ARG_ID:
                raise_exception(20);
                break;

            //flag handling
            case OVERWRITE_FLAG_ID:
                Info->overwrite_flag=1;
                break;
            case VERSION_FLAG_ID:
                Info->version_flag=1;
                break;
            case HELP_FLAG_ID:
                Info->help_flag=1;
                break;

            case DEBUG_FLAG_ID:
                Info->debug_mode=1;
                break;
        }
    }
}

void raise_exception(int error_code){
    free(arg_mask);
    parser_exception(error_code);
}