/*
// Created by yotam on 25/03/2023.
*/

#ifndef UNTITLED1_OUTPUT_H
#define UNTITLED1_OUTPUT_H

#define BITS_IN_WORD 14

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "main.h"
#include "encoding.h"

extern bool makeExtFile;
extern bool makeEntFile;

void assemblerOutput(char *);
void printImage(FILE *);
void printEntries(FILE *);
void printExterns(FILE *);

#endif
