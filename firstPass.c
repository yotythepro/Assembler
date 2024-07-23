

#include "firstPass.h"

int pass1(FILE *fhand) {
    int ic = 0, dc = 0, lineNum = 0;
    char line[MAX_LINE + 1];
    const char delim[] = " \t\n\r";
    label *p;
    bool errors = false;
    rewind(fhand);
    while(fgets(line, MAX_LINE + 1, fhand)) {
        int i, l;
        bool hasLabel = false;
        char *temp = callok(MAX_LINE + 1, sizeof(char)), *word1, *word2, *labelStr;
        strcpy(temp, line);
        word1 = strtok(temp, delim);
        word2 = strtok(NULL, delim);
        ++lineNum;
        if(line[0] == ';') {
            continue;
        }
        if(!word1){
            continue;
        }
        if(!isValidLine(line, lineNum)) {
            errors = true;
            continue;
        }
        if(isLabel(word1)) {
            hasLabel = true;
            labelStr = word1;
            word1 = word2;
        }
        if(!strcmp(word1, ".data") || !strcmp(word1, ".string")) {
            char **argStrs = callok(100, sizeof(char *));
            data *args = callok(MAX_LINE, sizeof(data));
            int count;
            if(hasLabel) {
                label *newLabel = callok(1, sizeof(label));
                newLabel->name = labelStr;
                newLabel->value = dc;
                newLabel->type = dataLabel;
                newLabel->next = labelTable;
                newLabel->isEntry = false;
                newLabel->usageCount = 0;
                labelTable = newLabel;
            }
            count = getArgs(line, argStrs, (int) (word1 - temp), 100);
            extractData(argStrs, args, count);
            l = encodeData(args, count, dc);
            dc += l;
            continue;
        }
        if(!strcmp(word1, ".extern") || !strcmp(word1, ".entry")) {
            if(!strcmp(word1, ".extern")) {
                char **args = callok(100, sizeof(char *));
                int count, j;
                count = getArgs(line, args, (int) (word1 - temp), 100);
                for(j = 0; j < count; j++) {
                    label *newLabel = callok(1, sizeof(label));
                    newLabel->name = args[j];
                    newLabel->value = 0;
                    newLabel->type = externalLabel;
                    newLabel->next = labelTable;
                    newLabel->isEntry = false;
                    newLabel->usageCount = 0;
                    labelTable = newLabel;
                }
            }
            continue;
        }
        if(hasLabel) {
            label *newLabel = callok(1, sizeof(label));
            newLabel->name = labelStr;
            newLabel->value = ic;
            newLabel->type = codeLabel;
            newLabel->next = labelTable;
            newLabel->isEntry = false;
            newLabel->usageCount = 0;
            labelTable = newLabel;
        }
        for(i = 0; i < len(COMMANDS); i++) {
            if(!strcmp(word1, COMMANDS[i].name)) {
                int paramCount;
                char **paramStrs = callok(2, sizeof(char *));
                char **argStrs = callok(2, sizeof(char *));
                argType *types = callok(2, sizeof(argType));
                int count;
                count = getArgs(line, argStrs, (int) (word1 - temp), 2);
                paramCount = getParams(line, paramStrs, (int) (word1 - temp));
                extractTypes(argStrs, types, count);
                if(!strcmp(COMMANDS[i].name, "jmp") || !strcmp(COMMANDS[i].name, "bne") || !strcmp(COMMANDS[i].name, "jsr")) {
                    argType *paramTypes = callok(2, sizeof(argType));
                    extractTypes(paramStrs, paramTypes, paramCount);
                    l = encodeCmd(COMMANDS[i], types, count, paramTypes, paramCount, ic);
                    break;
                }
                l = encodeCmd(COMMANDS[i], types, count, NULL, 0, ic);
                break;
            }
        }
        ic += l;
    }
    if(errors)
        return 1;
    p = labelTable;
    while(p != NULL) {
        if (p->type == dataLabel)
            p->value += ic;
        p = p->next;
    }
    /*printLabels();*/
    return 0;
}
