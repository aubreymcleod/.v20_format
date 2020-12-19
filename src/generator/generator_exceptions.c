//
// Created by Aubrey McLeod on 2020-12-15.
//

#include <stdlib.h>
#include <stdio.h>
#include "generator_exceptions.h"

#define ERROR_OFFSET 2000


void generator_exception(int error_code) {
    char *error_msg;
    switch (error_code) {
        case 1:
            error_msg = "Unable to open the file designated for $2000";
            break;
        case 2:
            error_msg = "Unable to open the file designated for $4000";
            break;
        case 3:
            error_msg = "Unable to open the file designated for $6000";
            break;
        case 4:
            error_msg = "Unable to open the file designated for $A000";
            break;
        case 5:
            error_msg = "Unable to open the file designated for $B000";
            break;
        case 6:
            error_msg = "Unable to open one of the unspecified input files";
            break;
        case 10:
            error_msg = "A given input file is insufficiently short (less than 4k)";
            break;
        case 11:
            error_msg = "Given ROM file is of strange length; unsure of how to proceed.";
            break;
        case 20:
            error_msg = "Multiple files attempted to occupy the same block, either implicitly or explicitly";
            break;
        case 21:
            error_msg = "Blocks in a given ROM file exceeded the contiguous space allotted, are you sure that this should be here?";
            break;
        case 22:
            error_msg = "Could not determine destination of unspecified file, please use destination flags with this file";
            break;
        case 30:
            error_msg = "Attempting to overwrite existing file, please use the -ow flag to allow overwrites";
            break;
        case 31:
            error_msg = "Could not write to specified output file.";
            break;
        default:
            error_msg = "Something went wrong when building your ROM";
            break;
    }
    printf("ROM BUILDER Error: %s", error_msg);
    exit(error_code+ERROR_OFFSET);
}

