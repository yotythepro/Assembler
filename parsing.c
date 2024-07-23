

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parsing.h"
#include "macro.h"
#include "labels.h"

int getArgs(const char *line, char **argArr, int cmdIndex, int max) {
    int curr = cmdIndex, i = 0, j;
    bool inParams = false;
    while(line[curr] != ' ' && line[curr] != '\t' && line[curr] != '\n')
        ++curr;
    SKIP_WHITESPACE
    if(line[curr] == '\n')
        return 0;
    if(line[curr] == ',') {
        printf("%d: fun fact! arguments should not start with a comma!\n", currTestedLineNum);
        return -1;
    }
    while(line[curr]) {
        argArr[i] = callok(MAX_LINE, sizeof(char));
        j = 0;
        while(line[curr] != ' ' && line[curr] != '\t' && line[curr] != '\n' && line[curr] != '\0' && (line[curr] != ',' || inParams) /*funcinize*/) {
            if(line[curr] == '(')
                inParams = true;
            if(line[curr] == ')')
                inParams = false;
            argArr[i][j] = line[curr];
            ++curr;
            ++j;
        }
        argArr[i][j] = '\0';
        ++i;
        SKIP_WHITESPACE
        if(line[curr] == '\n' || line[curr] == '\0')
            return i;
        if(line[curr] != ',') {
            printf("%d: if you could pls put a comma between arguments that'd be epic, thankss\n", currTestedLineNum);
            return -1;
        }
        ++curr;
        SKIP_WHITESPACE
        if(line[curr] == '\n' || line[curr] == '\0' || line[curr] == ',' || i == max) {
            printf("%d: don't place a comma unless u wanna place another arg..\n", currTestedLineNum);
            return -1;
        }
    }
    printf("what on earth are you doing? getArgs should not get to this line..\n"); /*migth make error catching easier, and removes warning*/
    return INT_MAX;
}

int getParams(const char *line, char **paramArr, int cmdIndex) {
    char **args = callok(2, sizeof(char *));
    int curr = 0, i = 0;
    for(; i < 2; ++i)
        args[i] = callok(MAX_LINE + 1, sizeof(char));
    i = 0;
    getArgs(line, args, cmdIndex, 2);
    while(args[0][curr] != '(') {
        if(args[0][curr] == '\0')
            return 0;
        ++curr;
    }
    ++curr;
    paramArr[0] = callok(MAX_LINE, sizeof(char));
    while(args[0][curr] != ',') { /*funcinize*/
        if(args[0][curr] == '\0' || args[0][curr] == ')' || args[0][curr] == '(') {
            printf("%d: failed to make 2, non-nested parameters\n", currTestedLineNum);
            return -1;
        }
        paramArr[0][i] = args[0][curr];
        ++curr;
        ++i;
    }
    paramArr[0][i] = '\0';
    ++curr;
    i = 0;
    paramArr[1] = callok(MAX_LINE, sizeof(char));
    while(args[0][curr] != ')') { /*funcinize*/
        if(args[0][curr] == '\0' || args[0][curr] == ',' || args[0][curr] == '(') {
            printf("%d: failed to make 2, non-nested parameters\n", currTestedLineNum);
            return -1;
        }
        paramArr[1][i] = args[0][curr];
        ++curr;
        ++i;
    }
    if(args[0][++curr] != '\0') {
        printf("%d: argument should not continue after parameters\n", currTestedLineNum);
        return -1;
    }
    paramArr[1][i] = '\0';
    return 2;
}

int extractData(char **src, data *dest, int n) {
    int i;
    if(src[0][0] == '"') { /*funcinize*/
        int curr = 1;
        dest[0].strval = callok(MAX_LINE + 1, sizeof(char));
        if(n != 1) {
            printf("%d: string argument must be only argument\n", currTestedLineNum);
            return 1;
        }
        while(src[0][curr] != '"') {
            if(src[0][curr] == '\0') {
                printf("%d: string argument not terminated properly\n", currTestedLineNum);
                return 1;
            }
            dest[0].strval[curr - 1] = src[0][curr];
            ++curr;
        }
        dest[0].strval[curr - 1] = '\0';
        dest[0].ival = INT_MAX;
        return 0;
    }
    for(i = 0; i < n; i++) { /*funcinize*/
        int val;
        val = getImmedIntVal(src[i]);
        if(val == INT_MAX) {
            printf("%d: argument '%s' not a valid number\n", currTestedLineNum, src[i]);
            return 1;
        }
        dest[i].ival = val;
        dest[i].strval = NULL;
    }
    return 0;
}

int extractTypes(char **src, argType *dest, int n) {
    int i;
    for(i = 0; i < n; i++) {
        bool found = false;
        if(src[i][0] == '#') { /*funcinize*/
            ++src[i];
            if(getImmedIntVal(src[i]) == INT_MAX) {
                printf("%d: argument '%s' not a valid number\n", currTestedLineNum, src[i]);
                return 1;
            }
            dest[i] = value;
            continue;
        }
        if(isRegisterName(src[i]) != -1) {
            dest[i] = reg;
            continue;
        }
        if(currPhase == firstPass)
            dest[i] = local;
        else {
            label *p = labelTable;
            while(p) {
                if(!strcmp(src[i], p->name)) {
                    dest[i] = (p->type == externalLabel) ? external : local;
                    found = true;
                    break;
                }
                p = p->next;
            }
            if(!found) {
                printf("%d: undefined argument '%s'\n", currTestedLineNum, src[i]);
                return 1;
            }
        }
    }
    return 0;
}

void trimParams(char *arg) {
    strtok(arg, "(");
}

int isRegisterName(char *str) {
    int i;
    for(i = 0; i < REGISTERS; i++) {
        char regName[3] = {'r', '0', '\0'};
        regName[1] += i;
        if(!strcmp(str, regName))
            return i;
    }
    return -1;
}

int getImmedIntVal(char *str) {
    int mult = 1, val;
    char *ptr;
    if(str[0] == '-') {
        mult = -1;
        ++str;
    } else if(str[0] == '+') {
        ++str;
    }
    if(!isdigit(str[0]))
        return INT_MAX;
    val = strtol(str, &ptr, 10);
    if(ptr[0] != '\0')
        return INT_MAX;
    return val * mult;
}
