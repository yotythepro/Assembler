/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

phase currPhase;

int main (int argc, char *argv[]) {
    int i;
    FILE *fhand;
    if(argc == 1) {
        fprintf(stderr, "Please enter file name as argument\n");
        exit(1);
    }
    for(i = 1; i < argc; ++i) {
        char fname[FILENAME_MAX];
        makeEntFile = false;
        makeExtFile = false;
        labelTable = NULL;
        strcpy(fname, argv[i]);
        strcat(fname, ".as");
        fhand = fopen(fname, "r");
        if(!fhand) {
            fprintf(stderr, "File is existan't\n");
            exit(1);
        }
        currPhase = preProcessor;
        fhand = preAssemble(fhand, argv[i]);
        currPhase = firstPass;
        if(pass1(fhand)) {
            fclose(fhand);
            continue;
        }
        currPhase = secondPass;
        if(pass2(fhand)) {
            fclose(fhand);
            continue;
        }
        currPhase = output;
        assemblerOutput(argv[i]);
        fclose(fhand);
    }
    freeAll();
    return 0;
}

