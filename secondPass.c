

#include "secondPass.h"

int pass2(FILE *fhand) {
    int ic = 0, lineNum = 0;
    char line[MAX_LINE + 1];
    const char delim[] = " \t\n\r";
    bool errors = false;
    rewind(fhand);
    while(fgets(line, MAX_LINE + 1, fhand)) {
        char *temp = callok(MAX_LINE + 1, sizeof(char)), *word1, *word2;
        char **args = callok(2, sizeof(char *)), **params = callok(2, sizeof(char *));
        int argCount, paramCount;
        strcpy(temp, line);
        word1 = strtok(temp, delim);
        word2 = strtok(NULL, delim);
        ++lineNum;
        if(line[0] == ';')
            continue;
        if(!word1)
            continue;
        if(!isValidLine(line, lineNum)) {
            errors = true;
            continue;
        }
        if(isLabel(word1))
            word1 = word2;
        if(!strcmp(word1, ".data") || !strcmp(word1, ".string") || !strcmp(word1, ".extern"))
            continue;
        if(!strcmp(word1, ".entry")) {
            char *arg = callok(MAX_LABEL_NAME + 1, sizeof(char));
            label *p = labelTable;
            makeEntFile = true;
            getArgs(line, &arg, word1 - temp, 1);
            while(p) {
                if(!strcmp(p->name, arg)) {
                    p->isEntry = true;
                    break;
                }
                p = p->next;
            }
            continue;
        }
        argCount = getArgs(line, args, word1 - temp, 2);
        paramCount = getParams(line, params, word1 - temp);
        if(paramCount)
            trimParams(args[0]);
        ic += encodeArgs(args, argCount, params, paramCount, ic);
    }
    if(errors)
        return 1;
    return 0;
}