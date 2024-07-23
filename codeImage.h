

#ifndef UNTITLED1_CODEIMAGE_H
#define UNTITLED1_CODEIMAGE_H

#define IMMED      0
#define DIRECT     1
#define PARAM      2
#define REG_DIRECT 3

#define ABSOLUTE    0
#define EXTERNAL    1
#define RELOCATABLE 2


typedef struct{
    unsigned int ARE : 2;
    unsigned int dest : 2;
    unsigned int src : 2;
    unsigned int opcode : 4;
    unsigned int param2 : 2;
    unsigned int param1 : 2;
} instructionWord;

typedef struct{
    unsigned int ARE : 2;
    unsigned int dest : 6;
    unsigned int src : 6;
} registerWord;

typedef struct{
    unsigned int ARE : 2;
    unsigned int address : 12;
} labelWord;

typedef labelWord immedWord;

typedef union {
    labelWord lVal;
    registerWord rVal;
    instructionWord inVal;
    immedWord imVal;
    int ival;
} word;

extern word codeImage[500];

#endif
