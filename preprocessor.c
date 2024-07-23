#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "macro.h"
#include "strOps.h"

/*
// Created by yotam on 13/02/2023.
*/


FILE *preAssemble(FILE * fhand, char *fname) {
    Macro *head = newMacro();
    Macro *last = head;
    char line[MAX_LINE + 1];
    const char delim[] = " \t\n";
    char outName[FILENAME_MAX];
    FILE * preAssembled;
    strcpy(outName, fname);
    strcat(outName, ".am");
    preAssembled = fopen(outName, "w");
    while(fgets(line, MAX_LINE + 1, fhand)) {
        char toPrint[MAX_LINE + 1];
        char *word1;
        /*export to function*/
        Macro *p = head;
        bool macroInList = false;
        strcpy(toPrint, line);
        word1 = strtok(line, delim);
        if(!word1) {
            continue;
        }
        while(p != NULL) {
            if(!strcmp(word1, p->name)) {
                macroInList = 1;
                break;
            }
            p = p->next;
        }
        if(macroInList) {
            fprintf(preAssembled, "%s", p->contents);
            continue;
        }
        if(!strcmp(word1, "mcr")) {
            char *name = strtok(NULL, delim);
            Macro *curr = (Macro *) callok(1, sizeof(Macro));
            bool ended = false;
            strcpy(curr->name, name);
            curr->contents = callok(999, sizeof(char));
            while(1) {
                char temp[MAX_LINE + 1];
                if(!fgets(line, MAX_LINE + 1, fhand))
                    break;
                strcpy(temp, line);
                word1 = strtok(temp, delim);
                if(!strcmp(word1, "endmcr")) {
                    ended = true;
                    break;
                }
                strcat(curr->contents, line);
            }
            if(ended) {
                last->next = curr;
                last = curr;
                continue;
            }
        }
        fprintf(preAssembled, "%s", toPrint);
    }
    fclose(fhand);
    fclose(preAssembled);
    return fopen(outName, "r");
}