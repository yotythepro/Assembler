/*
// Created by yotam on 25/02/2023.
*/

#include "macro.h"
#include <stdio.h>
#include <stdlib.h>

Macro *newMacro() {
    Macro *curr;
    curr = (Macro *) callok(1, sizeof(Macro));
    curr->contents = callok(999, sizeof(char));
    return curr;
}

