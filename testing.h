

#ifndef UNTITLED1_TESTING_H
#define UNTITLED1_TESTING_H

#include "labels.h"
#include "datalmage.h"
#include "codeImage.h"
#include <stdlib.h>

void printLabels();
void printCodeImage(int);
void printDataImage(int);
void freeAll();

extern void *toFree[2000];
extern int allocedNum;

#endif 
