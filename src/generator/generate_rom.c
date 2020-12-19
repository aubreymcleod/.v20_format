//
// Created by Aubrey McLeod on 2020-12-15.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "generate_rom.h"
#include "generator_exceptions.h"

const char *file_token = "VIC-20v";
const uint8_t version_number = VERSION;


typedef struct block {
    uint8_t byte[BLOCK_SIZE];
} block;


typedef struct input_file{
    FILE *fp;
    uint8_t occupies;
    long length;
    int contained_blocks;
    int extra_bytes;
}input_file;



block *blocks;
uint8_t existant_blocks;
uint8_t header[HEADER_LENGTH];

void map_file(input_file*, uint8_t);
long get_file_len(FILE*);
uint8_t get_file_target(input_file*);

void load_bytes(input_file[], uint8_t);
void build_header(cli_info, char*[]);
uint8_t generate_option_flag(cli_info);
void write_file(FILE*);

bool test_file_exists(char*);

void raise_generator_exception(int);

void make_rom(cli_info info, char *argv[]){
    blocks=(block*)calloc(TOTAL_BLOCKS, sizeof(block)); //2000 = 0, 3000=1, 4000=2, 5000=3, 6000=4, 7000=5, A000=6, B000=7
    existant_blocks = 0;

    int known_file_qty = 0;
    //input_file *inputs=(input_file*)calloc(info.total_input_qty, sizeof(input_file));
    input_file inputs[info.total_input_qty];
    FILE *output;

    for(int i = 0; i<info.total_input_qty; i++){
        inputs[i].fp = NULL;
        inputs[i].occupies = 0;
        inputs[i].extra_bytes = 0;
        inputs[i].contained_blocks = 0;
        inputs[i].length = 0;
    }

    if(info.index_20) {
        if((inputs[known_file_qty].fp = fopen(argv[info.index_20], "rb")) == NULL)
            raise_generator_exception(1);

        map_file(&inputs[known_file_qty], 0b00000001);
        known_file_qty++;
    }

    if(info.index_40) {
        if((inputs[known_file_qty].fp = fopen(argv[info.index_40], "rb")) == NULL)
            raise_generator_exception(2);

        map_file(&inputs[known_file_qty], 0b00000100);
        known_file_qty++;
    }

    if(info.index_60) {
        if((inputs[known_file_qty].fp = fopen(argv[info.index_60], "rb")) == NULL)
            raise_generator_exception(3);

        map_file(&inputs[known_file_qty], 0b00010000);
        known_file_qty++;
    }

    if(info.index_a0) {
        if((inputs[known_file_qty].fp = fopen(argv[info.index_a0], "rb")) == NULL)
            raise_generator_exception(4);

        map_file(&inputs[known_file_qty], 0b01000000);
        known_file_qty++;
    }

    if(info.index_b0) {
        if((inputs[known_file_qty].fp = fopen(argv[info.index_b0], "rb")) == NULL)
            raise_generator_exception(5);

        map_file(&inputs[known_file_qty], 0b10000000);
        known_file_qty++;
    }

    for(int i = 0; i<info.unknown_input_qty; i++){
        if((inputs[known_file_qty+i].fp = fopen(argv[info.unknown_inputs[i]], "rb")) == NULL)
            raise_generator_exception(6);

        map_file(&inputs[known_file_qty+i], 0b00000000);
    }

    load_bytes(inputs, info.total_input_qty);
    //load bytes into memory
    build_header(info, argv);
    //writefile
    if(test_file_exists(argv[info.output_index]) && info.overwrite_flag == 0)
        raise_generator_exception(30);
    output = fopen(argv[info.output_index], "wb");
    write_file(output);

    for(int i = 0; i<info.total_input_qty; i++)
        fclose(inputs[i].fp);
    fclose(output);
    free(blocks);
}


void map_file(input_file *file, uint8_t target){
    file->length = get_file_len(file->fp);
    file->contained_blocks = file->length/BLOCK_SIZE;
    file->extra_bytes = file->length%BLOCK_SIZE;

    int available_space = 0;

    if(file->contained_blocks==0)
        raise_generator_exception(10);
    if(file->extra_bytes != 0 && file->extra_bytes != 2)
        raise_generator_exception(11);

    if(target == 0x00){ //unknown target
        if(file->extra_bytes<2)
            raise_generator_exception(22);
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
    if(available_space-file->contained_blocks<0)
        raise_generator_exception(21);

    for(int i = 1; i< file->contained_blocks; i++)
        target |= target<<1;

    if(target & existant_blocks)
        raise_generator_exception(20);

    if(file->extra_bytes>=2 && !(target & get_file_target(file)))
        printf("WARNING: file appears to contain a block destination that does not match what you specified.");

    file->occupies = target;
    existant_blocks |= target;
}

long get_file_len(FILE *fp){
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size;
}

uint8_t get_file_target(input_file *file){
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

void load_bytes(input_file input[], uint8_t input_qty){
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
                blocks[block_address].byte[c]=buffer[c];
            block_number++;
            input[i].occupies &= ~(current_block);
        }
    }
}

void build_header(cli_info info, char *argv[]){
    int offset = 0;
    for(int i = offset; i<strlen(file_token)+offset; i++)
        header[i] = file_token[i-offset];
    offset+=7;

    header[offset] = version_number;
    offset +=1;

    unsigned long name_length = strlen((info.name_index>0) ? argv[info.name_index] : "");
    for(int i = offset; i<32+offset; i++)
        header[i] = (i<name_length+offset) ? argv[info.name_index][i-offset] : 0;
    offset+=32;

    unsigned long autoexec_length = strlen((info.autoexec_index>0) ? argv[info.autoexec_index] : "");
    for(int i = offset; i<22+offset; i++)
        header[i] = (i<autoexec_length+offset) ? argv[info.autoexec_index][i-offset] : 0;
    offset+=22;

    header[offset] = existant_blocks;
    offset+=1;

    uint8_t option_flag = generate_option_flag(info);
    header[offset] = option_flag;
}

uint8_t generate_option_flag(cli_info info){
    uint8_t option_flag = 0;

    //video_mode
    option_flag |= (info.video_mode) ? 0b00000001: 0;

    return option_flag;
}

void write_file(FILE *output){
    //DEBUG
    for(int i = 0; i<TOTAL_BLOCKS; i++){
        printf("%x ", blocks[i].byte[0]);
    }
    //DEBUG

    fwrite(header, sizeof(uint8_t), HEADER_LENGTH, output);
    for(int i = 0; i<TOTAL_BLOCKS; i++){
        if(existant_blocks & 0b00000001<<i)
            fwrite(blocks[i].byte, sizeof(uint8_t), BLOCK_SIZE, output);
    }

}

bool test_file_exists(char *path){
    FILE *fp;
    if((fp = fopen(path, "r")) != NULL){
        fclose(fp);
        return true;
    }
    return false;
}

void raise_generator_exception(int error_code){
    free(blocks);
    generator_exception(error_code);
}