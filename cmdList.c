

#include "cmdList.h"

const Cmd COMMANDS[16] = {
        {"mov", 0, 2, {VAR | REG, DATA | VAR | REG}},
        {"cmp", 1, 2, {DATA | VAR | REG, DATA | VAR | REG}},
        {"add", 2, 2, {VAR | REG, DATA | VAR | REG}},
        {"sub", 3, 2, {VAR | REG, DATA | VAR | REG}},
        {"not", 4, 1, {VAR | REG}},
        {"clr", 5, 1, {VAR | REG}},
        {"lea", 6, 2, {VAR | REG, VAR}},
        {"inc", 7, 1, {VAR | REG}},
        {"dec", 8, 1, {VAR | REG}},
        {"jmp", 9, 1, {VAR | REG}},
        {"bne", 10, 1, {VAR | REG}},
        {"red", 11, 1, {VAR | REG}},
        {"prn", 12, 1, {DATA | VAR | REG}},
        {"jsr", 13, 1, {VAR | REG}},
        {"rts", 14, 0, {0}},
        {"stop", 15, 0, {0}}
        /*more stuff*/
};
