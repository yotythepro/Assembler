

#include <stdlib.h>
#include "validation.h"
#include "testing.h"

int currTestedLineNum = 0;

bool isLabel(char *str) {
    if(str[strlen(str) - 1] == ':') {
        str[strlen(str) - 1] = '\0';
        return true;
    }
    return false;
}

bool isValidLabel(char *str, int lineNum) {
    int i;
    if(strlen(str) > MAX_MACRO_NAME + 1) {
        printf("%d: label %s too long\n", lineNum, str);
        return false;
    }
    if(!isalpha(str[0])) {
        printf("%d: label %s starts with non-letter character\n", lineNum, str);
        return false;
    }
    for(i = 1 /*hehe, did 1 instead of 0 because efficient MWAHAHAHA*/; i < strlen(str); ++i) {
        if(!isalnum(str[i])) {
            printf("%d: label %s has non-alphanumeric character(s)\n", lineNum, str);
            return false;
        }
    }
    if(isReservedWord(str)) {
        printf("%d: label name %s is a reserved word\n", lineNum, str);
        return false;
    }
    return true;
}

bool isReservedWord(char *str) {
    int i;
    for(i = 0; i < len(COMMANDS); i++) {
        if(!strcmp(str, COMMANDS[i].name))
            return true;
    }
    for(i = 0; i < REGISTERS; i++) {
        char regName[3] = {'r', '0', '\0'};
        regName[1] += i;
        if(!strcmp(str, regName))
            return true;
    }
    if(getLabel(str))
        return true;
    return false;
}

bool isValidLine(const char *line, int lineNum) {
    const char delim[] = " \t\n";
    char *word1;
    char *temp = callok(MAX_LINE + 1, sizeof(char));
    currTestedLineNum = lineNum;
    strcpy(temp, line);
    word1 = strtok(temp, delim);
    if(word1 != NULL && isLabel(word1)) {
        if(word1 == NULL || (currPhase != secondPass && !isValidLabel(word1, lineNum)))
            return false;
        word1 = strtok(NULL, delim);
    }
    if(verifyCommand(word1, line, word1 - temp, lineNum))
        return true;
    if(isDataCmd(word1)) {
        return verifyDataCmd(word1, line, word1 - temp, lineNum);
    }
    return false;
}

bool verifyCommand(const char *cmd, const char *line, int index, int lineNum) {
    int i;
    for(i = 0; i < len(COMMANDS); i++) {
        if(!strcmp(cmd, COMMANDS[i].name)) {
            char **argStrs = callok(2, sizeof(char *)), **paramStrs = callok(2, sizeof(char *));
            argType *types = callok(2, sizeof(argType)), *paramTypes = callok(2, sizeof(argType));
            int count, paramCount;
            Cmd currCmd = COMMANDS[i];
            if((count = getArgs(line, argStrs, index, 2)) == -1)
                return false;
            if((paramCount = getParams(line, paramStrs, index)) == -1)
                return false;
            if(paramCount != 0) {
                if(strcmp(COMMANDS[i].name, "jmp") && strcmp(COMMANDS[i].name, "bne") && strcmp(COMMANDS[i].name, "jsr")) {
                    printf("%d: parameters given for non-param command\n", lineNum);
                    return false;
                }
                if(extractTypes(paramStrs, paramTypes, paramCount))
                    return false;
                trimParams(argStrs[0]);
            }
            if(extractTypes(argStrs, types, count))
                return false;
            if(count != COMMANDS[i].argCount) {
                printf("%d: incorrect number of arguments for %s, expected: %d found: %d\n", lineNum, COMMANDS[i].name, COMMANDS[i].argCount, count);
                return false;
            }
            for(i = 0; i < count; i++) {
                AcceptedTypes currActual;
                switch (types[i]) { /*funcinize*/
                    case value:
                        currActual = DATA;
                        break;
                    case local:
                    case external:
                        currActual = VAR;
                        break;
                    case reg:
                        currActual = REG;
                }
                if(!(currActual & currCmd.argTypes[i])) {
                    printf("%d: wrong argument types for command %s\n", lineNum, currCmd.name);
                    return false;
                }
            }
            return true;
        }
    }
    if(!isDataCmd(cmd))
        printf("%d: '%s' is not a valid command name\n", lineNum, cmd);
    return false;
}

bool isDataCmd(const char *cmd) {
    return (!strcmp(cmd, ".data") || !strcmp(cmd, ".string") || !strcmp(cmd, ".entry") || !strcmp(cmd, ".extern"));
}

bool verifyDataCmd(const char *word1, const char *line, int index, int lineNum) {
    char **argStrs = callok(100, sizeof(char *));
    data *dataArr = callok(100, sizeof(data));
    int count, i;
    for(i = 0; i < 100; i++)
        argStrs[i] = callok(100, sizeof(char));
    if((count = getArgs(line, argStrs, index, 100)) <= 0)
        return false;
    if(!strcmp(word1, ".data") ||  !strcmp(word1, ".string")) {
        if(extractData(argStrs, dataArr, count))
            return false;
        if(!strcmp(word1, ".data") && dataArr[0].ival != INT_MAX)
            return true;
        if(!strcmp(word1, ".string") && dataArr[0].ival == INT_MAX)
            return true;
        printf("%d: Wrong data type for command \"%s\"\n", lineNum, word1);
        return false;
    }
    if(!strcmp(word1, ".extern")) {
        if(currPhase == secondPass)
            return true;
        if(!isValidLabel(argStrs[0], lineNum))
            return false;
        if(count != 1) {
            printf("%d: Incorrect number of arguments for command \"%s\"\n", lineNum, word1);
            return false;
        }
        return true;
    }
    if(!strcmp(word1, ".entry")) {
        label *p = labelTable;
        bool found = false;
        if(count != 1) {
            printf("%d: Incorrect number of arguments for command \"%s\"\n", lineNum, word1);
            return false;
        }
        if(currPhase == firstPass)
            return true;
        while(p) {
            if(!strcmp(argStrs[0], p->name)) {
                found = true;
                break;
            }
            p = p->next;
        }
        if(!found) {
            printf("%d: Label \"%s\" is not defined\n", lineNum, word1);
            return false;
        }
        return true;
    }
    return false; /*"NOn-vOiD FuNCtIon dOeS nOt rETuRn a VaLUe In aLl cONtRoL pAtHS" SHUT UP*/
}

void *callok(size_t n, size_t sz) {
    void *mem = calloc(n, sz);
    if(mem == NULL) {
        printf("memory no workie\n");
        exit(3);
    }
    toFree[allocedNum] = mem;
    ++allocedNum;
    return mem;
}