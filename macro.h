/*
// Created by yotam on 25/02/2023.
*/

#ifndef MACRO_H
#define MACRO_H

#include "main.h"

typedef struct macro {
    char name[MAX_MACRO_NAME + 1];
    char *contents;
    struct macro *next;
} Macro;

Macro *newMacro();

#endif 



