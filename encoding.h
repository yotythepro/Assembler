

#ifndef UNTITLED1_ENCODING_H
#define UNTITLED1_ENCODING_H

#include <stdlib.h>
#include <limits.h>
#include "parsing.h"
#include "datalmage.h"
#include "codeImage.h"
#include "main.h"
#include "labels.h"


int encodeData(data *, int, int);
int encodeCmd(Cmd, argType *, int, argType *, int, int);
int encodeArgs(char **, int, char **, int, int);

extern int totalCode;
extern int totalData;

#endif 
