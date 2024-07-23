

#ifndef UNTITLED1_PARSING_H
#define UNTITLED1_PARSING_H


#include <limits.h>

#define SKIP_WHITESPACE \
    while(line[curr] == ' ' || line[curr] == '\t') \
        ++curr;

typedef enum ArgType{value, local, external, reg} argType;
typedef struct Data {
    int ival;
    char *strval;
} data;

#include "cmdList.h"
#include "validation.h"

int getArgs(const char *, char **, int, int);
int getParams(const char *, char **, int);
int extractData(char **, data *, int);
int extractTypes(char **, argType *, int);
void trimParams(char *);
void *callok(size_t, size_t);
int isRegisterName(char *);
int getImmedIntVal(char *);

#endif 



