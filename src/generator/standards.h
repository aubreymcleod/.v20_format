//
// Created by Aubrey McLeod on 2020-12-21.
//

#ifndef MAKEV20_STANDARDS_H
#define MAKEV20_STANDARDS_H

#include "../datatypes.h"

rom_standard_t lookup_standards(char *name);

const rom_standard_t standards_definitions[1];
const uint16_t supported_standards;

#endif //MAKEV20_STANDARDS_H
