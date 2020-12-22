//
// Created by Aubrey McLeod on 2020-12-21.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "generator.h"
#include "generator/standards.h"


static file_block_t *block_rules = NULL;
static uint16_t number_of_rules = 0;
static file_info_t file_info = {0};

void load_rules(char *version_num){
    free(block_rules);
    block_rules = NULL;

    rom_standard_t standard = lookup_standards(version_num);
    block_rules = standard.block_rules;
    number_of_rules = standard.number_of_rules;
}




/** IO section **/
/* determines whether or not a given file path already exists */
bool test_file_exists(char *path){
    FILE *fp;
    if((fp = fopen(path, "r")) != NULL){
        fclose(fp);
        return true;
    }
    return false;
}

/* gets the length of a rule */
long get_file_len(FILE *fp){
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size;
}

/* Attempts to read the second byte of a given ROM file to determine the load address */
uint8_t get_file_target(input_file_t *file){
    uint8_t target;

    uint8_t buffer[2];
    fread(buffer, sizeof(uint8_t), 2, file->fp);
    switch(buffer[1]) {
        case 0x20:
            target = 0b00000001;
            break;
        case 0x40:
            target = 0b00000100;
            break;
        case 0x60:
            target = 0b00010000;
            break;
        case 0xA0:
            target = 0b01000000;
            break;
        case 0xB0:
            target = 0b10000000;
            break;
        default:
            target = 0x00000000;
            break;
    }
    fseek(file->fp, 0L, SEEK_SET);
    return target;
}

/* creates a map of a given file, where it sits in terms which blocks it occupies.*/
void map_file(input_file_t *file, uint8_t target){
    file->length = get_file_len(file->fp);
    file->contained_blocks = file->length/BLOCK_SIZE;
    file->extra_bytes = file->length%BLOCK_SIZE;

    int available_space = 0;

    if(file->contained_blocks == 0) {
        printf("A given input file is insufficiently short (less than 4k)");
        exit(150);
    }
    if(file->extra_bytes != 0 && file->extra_bytes != 2) {
        printf("Given ROM file is of strange length; unsure of how to proceed.");
        exit(151);
    }

    if(target == 0x00){ //unknown target
        if(file->extra_bytes<2) {
            printf("Could not determine destination of file, please use destination flags with this file");
            exit(160);
        }
        target = get_file_target(file);
    }

    switch(target){
        case 0b00000001: //target 2000
            available_space = 6;
            break;
        case 0b00000100: //target 4000
            available_space = 4;
            break;
        case 0b00010000: //target 6000
        case 0b01000000: //target a000
            available_space = 2;
            break;
        case 0b10000000:
            available_space = 1;
            break;
    }
    if(available_space-file->contained_blocks<0){
        printf("Blocks in a given ROM file exceeded the contiguous space allotted, are you sure that this should be here?");
        exit(161);
    }

    for(int i = 1; i< file->contained_blocks; i++)
        target |= target<<1;

    if(target & file_info.existant_blocks){
        printf("Multiple files attempted to occupy the same block, either implicitly or explicitly");
        exit(162);
    }

    if(file->extra_bytes>=2 && !(target & get_file_target(file)))
        printf("WARNING: file appears to contain a block destination that does not match what you specified.");

    file->occupies = target;
    file_info.existant_blocks |= target;
}

void load_bytes(input_file_t input[], uint8_t input_qty){
    uint8_t current_block = 0;
    int block_address = 0;
    int offset;
    int block_number;
    uint8_t buffer[BLOCK_SIZE];
    for(uint8_t i = 0; i<input_qty; i++){
        block_number = 0;
        offset = 0;
        while(input[i].occupies){

            if(input[i].occupies & 0b00000001){ //block 2000
                current_block = 0b00000001;
                block_address = 0;
            }
            else if(input[i].occupies & 0b00000010){ //block 3000
                current_block = 0b00000010;
                block_address = 1;
            }
            else if(input[i].occupies & 0b00000100){ //block 4000
                current_block = 0b00000100;
                block_address = 2;
            }
            else if(input[i].occupies & 0b00001000){ //block 5000
                current_block = 0b00001000;
                block_address = 3;
            }
            else if(input[i].occupies & 0b00010000){ //block 6000
                current_block = 0b00010000;
                block_address = 4;
            }
            else if(input[i].occupies & 0b00100000){ //block 7000
                current_block = 0b00100000;
                block_address = 5;
            }
            else if(input[i].occupies & 0b01000000){ //block A000
                current_block = 0b01000000;
                block_address = 6;
            }
            else if(input[i].occupies & 0b10000000){ //block B000
                current_block = 0b10000000;
                block_address = 7;
            }

            if(input[i].extra_bytes == 2)
                offset = 2;

            fseek(input[i].fp, offset+(block_number*BLOCK_SIZE), SEEK_SET);
            fread(buffer, sizeof(buffer), 1, input[i].fp);
            fseek(input->fp, 0L, SEEK_SET);

            for(int c = 0; c<BLOCK_SIZE; c++)
                file_info.blocks[block_address].byte[c]=buffer[c];
            block_number++;
            input[i].occupies &= ~(current_block);
        }
    }
}



/** lite loading routines **/
/* load values which are directly derived from the CLI */
void populate_from_cli(cli_info_t cli, char *argv[], file_info_t *info){
    info->standard_version = lookup_standards(cli.standard_version).version_number;
    info->input_qty = cli.total_input_qty;
    info->video_mode |= cli.video_mode;
    info->rom_name = (cli.name_index != 0) ? argv[cli.name_index] : "";
    info->auto_exec = (cli.autoexec_index != 0) ? argv[cli.autoexec_index] : "";
}






void generate(cli_info_t info, char *argv[]){
    load_rules(info.standard_version);
    //populate values;
    populate_from_cli(info, argv, &file_info);

    //file handling/loading
    int known_file_qty = 0;
    for(int i = 0; i<ADDRESSABLE_LOCATIONS; i++){
        file_info.input[i].fp = NULL;
        file_info.input[i].occupies = 0;
        file_info.input[i].extra_bytes = 0;
        file_info.input[i].contained_blocks = 0;
        file_info.input[i].length = 0;
    }

    if(info.index_20) {
        if((file_info.input[known_file_qty].fp = fopen(argv[info.index_20], "rb")) == NULL){
            printf("Unable to open the file designated for $2000");
            exit(140);
        }

        map_file(&file_info.input[known_file_qty], 0b00000001);
        known_file_qty++;
    }

    if(info.index_40) {
        if((file_info.input[known_file_qty].fp = fopen(argv[info.index_40], "rb")) == NULL){
            printf("Unable to open the file designated for $4000");
            exit(141);
        }

        map_file(&file_info.input[known_file_qty], 0b00000100);
        known_file_qty++;
    }

    if(info.index_60) {
        if((file_info.input[known_file_qty].fp = fopen(argv[info.index_60], "rb")) == NULL){
            printf("Unable to open the file designated for $6000");
            exit(142);
        }

        map_file(&file_info.input[known_file_qty], 0b00010000);
        known_file_qty++;
    }

    if(info.index_a0) {
        if((file_info.input[known_file_qty].fp = fopen(argv[info.index_a0], "rb")) == NULL){
            printf("Unable to open the file designated for $A000");
            exit(143);
        }

        map_file(&file_info.input[known_file_qty], 0b01000000);
        known_file_qty++;
    }

    if(info.index_b0) {
        if((file_info.input[known_file_qty].fp = fopen(argv[info.index_b0], "rb")) == NULL){
            printf("Unable to open the file designated for $B000");
            exit(144);
        }

        map_file(&file_info.input[known_file_qty], 0b10000000);
        known_file_qty++;
    }

    for(int i = 0; i<info.unknown_input_qty; i++){
        if((file_info.input[known_file_qty+i].fp = fopen(argv[info.unknown_inputs[i]], "rb")) == NULL){
            printf("Unable to open one of the unspecified input files");
            exit(146);
        }

        map_file(&file_info.input[known_file_qty+i], 0b00000000);
    }


    load_bytes(file_info.input, info.total_input_qty);
    if(test_file_exists(argv[info.output_index]) && info.overwrite_flag == 0){
        printf("Attempting to overwrite existing file, please use the -ow flag to allow overwrites");
        exit(170);
    }

    file_info.output = fopen(argv[info.output_index], "wb");


    //write header
    for(int i = 0; i < number_of_rules; i++){
        block_rules[i].set_func(file_info, block_rules[i].length);
    }


    //write ROMs
    for(int i = 0; i<TOTAL_BLOCKS; i++) {
        if (file_info.existant_blocks & 0b00000001 << i) {
            fwrite(file_info.blocks[i].byte, sizeof(uint8_t), BLOCK_SIZE, file_info.output);
        }
    }

    //close all ROMs
    for(int i = 0; i < ADDRESSABLE_LOCATIONS; i++){
        if(file_info.input[i].fp != NULL){
            fclose(file_info.input[i].fp);
        }
    }
    fclose(file_info.output);
}