/*
// Created by User on 10/03/2023.
*/

#ifndef UNTITLED1_VALIDATION_H
#define UNTITLED1_VALIDATION_H

#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "main.h"
#include "cmdList.h"
#include "parsing.h"
#include "labels.h"

bool isLabel(char *);
bool isValidLabel(char *, int);
bool isReservedWord(char *);
bool isValidLine(const char *, int);
bool verifyCommand(const char *, const char *, int, int);
bool verifyDataCmd(const char *, const char *, int, int);
bool isDataCmd(const char *);

extern int currTestedLineNum;

#endif 
