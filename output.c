/*
// Created by yotam on 25/03/2023.
*/

#include "output.h"

bool makeExtFile = false;
bool makeEntFile = false;


void assemblerOutput(char *fname) {
    char outName[FILENAME_MAX], entFName[FILENAME_MAX], extFName[FILENAME_MAX];
    strcpy(outName, fname);
    strcat(outName, ".ob");
    strcpy(entFName, fname);
    strcat(entFName, ".ent");
    strcpy(extFName, fname);
    strcat(extFName, ".ext");
    printImage(fopen(outName, "w"));
    if(makeEntFile)
        printEntries(fopen(entFName, "w"));
    if(makeExtFile)
        printExterns(fopen(extFName, "w"));
}

void printImage(FILE *fhand) {
    int i, j;
    for(i = 0; i < totalCode; i++) {
        int curr = codeImage[i].ival;
        fprintf(fhand,"%d\t", i + 100);
        for(j = BITS_IN_WORD - 1; j >= 0; j--) {
            if(curr >= pow(2, j)) {
                curr -=pow(2, j);
                fprintf(fhand, "/");
            } else {
                fprintf(fhand, ".");
            }
        }
        fprintf(fhand, "\n");
    }
    for(i = 0; i < totalData; i++) {
        int curr = dataImage[i];
        fprintf(fhand,"%d\t", i + 100 + totalCode);
        for(j = BITS_IN_WORD - 1; j >= 0; j--) {
            if(curr >= pow(2, j)) {
                curr -=pow(2, j);
                fprintf(fhand, "/");
            } else {
                fprintf(fhand, ".");
            }
        }
        fprintf(fhand, "\n");
    }
    fclose(fhand);
}

void printEntries(FILE *fhand) {
    label *p = labelTable;
    while(p) {
        if(p->isEntry)
            fprintf(fhand, "%s\t%d\n", p->name, p->value);
        p = p->next;
    }
    fclose(fhand);
}

void printExterns(FILE *fhand) {
    label *p = labelTable;
    while(p) {
        if(p->type == externalLabel) {
            int i;
            for(i = 0; i < p->usageCount; i++)
                fprintf(fhand, "%s\t%d\n", p->name, p->usages[i]);
        }
        p = p->next;
    }
    fclose(fhand);
}