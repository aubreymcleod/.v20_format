//
// Created by Aubrey McLeod on 2020-12-21.
//

#include <string.h>
#include <stdio.h>
#include "version_1.h"

void generator_version_1_write_signature(file_info_t, uint16_t);
void generator_version_1_write_version(file_info_t, uint16_t);
void generator_version_1_write_name(file_info_t, uint16_t);
void generator_version_1_write_autoexec(file_info_t, uint16_t);
void generator_version_1_write_map(file_info_t, uint16_t);
void generator_version_1_write_video_mode(file_info_t, uint16_t);
void generator_version_1_write_reserved_space(file_info_t, uint16_t);


file_block_t version_1_block_rules[] = {
        {
            .name = "signature",
            .length = 7,
            .set_func = generator_version_1_write_signature
        },
        {
            .name = "version",
            .length = 1,
            .set_func = generator_version_1_write_version
        },
        {
            .name = "ROM name",
            .length = 32,
            .set_func = generator_version_1_write_name
        },
        {
            .name = "auto-exec",
            .length = 10,
            .set_func = generator_version_1_write_autoexec
        },
        {
            .name = "memory map",
            .length = 1,
            .set_func = generator_version_1_write_map
        },
        {
            .name = "video mode",
            .length = 1,
            .set_func = generator_version_1_write_video_mode
        },
        {
            .name = "reserved space",
            .length = 12,
            .set_func = generator_version_1_write_reserved_space
        }
};


void generator_version_1_write_signature(file_info_t info, uint16_t length){
    const char *signature = "VIC-20v";
    fwrite(signature, sizeof(char), length, info.output);
}

void generator_version_1_write_version(file_info_t info, uint16_t length){
    fwrite(&info.standard_version, sizeof(uint8_t), length, info.output);
}

void generator_version_1_write_name(file_info_t info, uint16_t max_length){
    unsigned int length = strlen(info.rom_name);
    /* if the name is too short (typically will be) */
    if(length < max_length){
        fwrite(info.rom_name, sizeof(char), length, info.output);

        /* fill the remainder of the space with 0s */
        for(int i = length; i < max_length; i++){
            fputc(0, info.output);
        }
    }
    else{
        fwrite(info.rom_name, sizeof(char), max_length, info.output);
    }
}

void generator_version_1_write_autoexec(file_info_t info, uint16_t max_length){
    unsigned int length = strlen(info.auto_exec);
    /* if the name is too short (typically will be) */
    if(length == 0){
        for(int i = 0; i<max_length; i++){
            fputc(0, info.output);
        }
    }
    else if(length < max_length){
        fwrite(info.auto_exec, sizeof(char), length, info.output);
        fputc('\n', info.output);
        for(int i = length+1; i<max_length; i++){
            fputc(0, info.output);
        }
    }
    else{
        fwrite(info.auto_exec, sizeof(char), max_length-1, info.output);
        fputc('\n', info.output);
    }
}

void generator_version_1_write_map(file_info_t info, uint16_t length){
    fwrite(&info.existant_blocks, sizeof(uint8_t), length, info.output);
}

void generator_version_1_write_video_mode(file_info_t info, uint16_t length){
    fputc(info.video_mode & 0x01, info.output);
}

void generator_version_1_write_reserved_space(file_info_t info, uint16_t length){
    for(int i = 0; i < length; i++){
        fputc(0, info.output);
    }
}

