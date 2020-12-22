//
// Created by Aubrey McLeod on 2020-12-21.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "standards.h"
#include "standards/version_1.h"

const uint16_t supported_standards = 1;
const rom_standard_t standards_definitions[] = {
        {
            .name = ".v20-1",
            .version_number = 1,
            .block_rules = version_1_block_rules,
            .number_of_rules=7
        }
};




rom_standard_t lookup_standards(char *name){
    for(uint16_t i = 0; i < supported_standards; i++){
        if(strcmp(name, standards_definitions[i].name) == 0){
            return standards_definitions[i];
        }
    }

    printf("Error: unable to identify \"%s\" as a valid standard", name);
    exit(200);
};