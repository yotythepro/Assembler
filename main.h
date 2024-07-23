#ifndef MAIN_ASSEMBLY_H
#define MAIN_ASSEMBLY_H


#include <stdio.h>

#define MAX_LINE 80
#define MAX_MACRO_NAME 30
#define MAX_LABEL_NAME 30
#define len(arr) sizeof(arr) / sizeof(arr[0])
#define REGISTERS 8

#include "codeImage.h"
#include "datalmage.h"



/*
// Created by yotam on 13/02/2023.
*/

typedef enum {false, true} bool;
typedef enum {preProcessor, firstPass, secondPass, output} phase;

extern phase currPhase;

FILE *preAssemble(FILE * fhand, char *fname);
int pass1(FILE *);
int pass2(FILE *);

#include "testing.h"
#include "output.h"

#endif