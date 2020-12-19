//
// Created by Aubrey McLeod on 2020-12-11.
//

#define ERROR_OFFSET 1000

#include "parser_execeptions.h"
#include <stdio.h>
#include <stdlib.h>

void parser_exception(int error_code) {
    char *error_msg;
    switch (error_code) {
        case 1:
            error_msg = "Arguments do not match command structure";
            break;
        case 2:
            error_msg = "Command flag that requires arguments lacks arguments";
            break;
        case 3:
            error_msg = "Too many input files specified";
            break;
        case 4:
            error_msg = "Too many output files specified";
            break;
        case 5:
            error_msg = "Too many rom names given";
            break;
        case 6:
            error_msg = "Too many autoexec commands given";
            break;
        case 7:
            error_msg = "Too many video formats given";
            break;
        case 12:
            error_msg = "Too many files addressed to $2000";
            break;
        case 13:
            error_msg = "Too many files addressed to $4000";
            break;
        case 14:
            error_msg = "Too many files addressed to $6000";
            break;
        case 15:
            error_msg = "Too many files addressed to $A000";
            break;
        case 16:
            error_msg = "Too many files addressed to $B000";
            break;
        case 20:
            error_msg = "Unknown Video Type given, expected \"pal\" or \"ntsc\"";
            break;
        case 30:
            error_msg = "Inputs given, but no output specified";
            break;
        case 31:
            error_msg = "Outputs given, but no inputs specified";
            break;
        default:
            error_msg = "Something went wrong when parsing your command";
            break;
    }
    printf("Command Parser Error: %s, please use the -help flag to see valid commands/arguments.", error_msg);
    exit(error_code+ERROR_OFFSET);
}