

#include "encoding.h"

int totalCode = 0;
int totalData = 0;

int encodeData(data *arr, int n, int dc) {
    int i, curr = dc;
    for(i = 0; i < n; i++) {
        if(arr[i].ival == INT_MAX) {
            int j;
            for(j = 0; arr[i].strval[j] != '\0'; j++)
                dataImage[curr++] = (unsigned char) arr[i].strval[j];
            dataImage[curr++] = 0;
            break;
        }
        dataImage[curr++] = arr[i].ival;
    }
    totalData = curr;
    return curr - dc;
}

int encodeCmd(Cmd command, argType *args, int count, argType *params, int paramCount, int ic) {
    int l = count + 1;
    bool regShortcut = false;
    codeImage[ic].inVal.ARE = 0;
    codeImage[ic].inVal.opcode = command.opCode;
    if(paramCount) {
        bool regShortcutParams = false;
        l += 2;
        codeImage[ic].inVal.dest = PARAM;
        codeImage[ic].inVal.src = 0;
        switch (params[0]) {
            case local:
            case external:
                codeImage[ic].inVal.param1 = DIRECT;
                break;
            case value:
                codeImage[ic].inVal.param1 = IMMED;
                break;
            case reg:
                regShortcutParams = true;
                codeImage[ic].inVal.param1 = REG_DIRECT;
                break;
        }
        switch (params[1]) {
            case local:
            case external:
                codeImage[ic].inVal.param2 = DIRECT;
                break;
            case value:
                codeImage[ic].inVal.param2 = IMMED;
                break;
            case reg:
                if(regShortcutParams)
                    l -= 1;
                codeImage[ic].inVal.param2 = REG_DIRECT;
                break;
        }
        return l;
    }
    codeImage[ic].inVal.param1 = 0;
    codeImage[ic].inVal.param2 = 0;
    codeImage[ic].inVal.dest = 0;
    codeImage[ic].inVal.src = 0;
    if(count == 1) {
        switch (args[0]) {
            case local:
            case external:
                codeImage[ic].inVal.dest = DIRECT;
                break;
            case value:
                codeImage[ic].inVal.dest = IMMED;
                break;
            case reg:
                regShortcut = true;
                codeImage[ic].inVal.dest = REG_DIRECT;
                break;
        }
    }
    if(count == 2) {
        switch (args[0]) {
            case local:
            case external:
                codeImage[ic].inVal.src = DIRECT;
                break;
            case value:
                codeImage[ic].inVal.src = IMMED;
                break;
            case reg:
                regShortcut = true;
                codeImage[ic].inVal.src = REG_DIRECT;
                break;
        }
        switch (args[1]) {
            case local:
            case external:
                codeImage[ic].inVal.dest = DIRECT;
                break;
            case value:
                codeImage[ic].inVal.dest = IMMED;
                break;
            case reg:
                if(regShortcut)
                    l -= 1;
                codeImage[ic].inVal.dest = REG_DIRECT;
                break;
        }
    }
    return l;
}

int encodeArgs(char **args, int count, char **params, int paramCount, int ic) {
    int l = count + paramCount + 1, curr = ic, i;
    bool lastIsReg = false;
    argType *argTypes = callok(count, sizeof(argType)), *paramTypes = callok(paramCount, sizeof(argType));
    label *lbl;
    extractTypes(args, argTypes, count);
    extractTypes(params, paramTypes, paramCount);
    for(i = 0; i < count; i++) {
        if(lastIsReg && (argTypes[i] == reg))
            l -= 1;
        else
            ++curr;
        switch (argTypes[i]) {
            case reg:
                lastIsReg = true;
                codeImage[curr].rVal.ARE = ABSOLUTE;
                if(i == 0 && count == 2)
                    codeImage[curr].rVal.src = isRegisterName(args[i]);
                else
                    codeImage[curr].rVal.dest = isRegisterName(args[i]);
                if(count == 1)
                    codeImage[curr].rVal.src = 0;
                break;
            case local:
                codeImage[curr].lVal.ARE = RELOCATABLE;
                codeImage[curr].lVal.address = getLabel(args[i])->value;
                break;
            case value:
                codeImage[curr].imVal.ARE = ABSOLUTE;
                codeImage[curr].imVal.address = getImmedIntVal(args[i]);
                break;
            case external:
                codeImage[curr].lVal.ARE = EXTERNAL;
                codeImage[curr].lVal.address = 0;
                lbl = getLabel(args[i]);
                lbl->usages[lbl->usageCount] = curr;
                ++(lbl->usageCount);
                makeExtFile = true;
                break;
        }
    }
    lastIsReg = false;
    for(i = 0; i < paramCount; i++) {
        if(lastIsReg && (paramTypes[i] == reg))
            l -= 1;
        else
            ++curr;
        switch (paramTypes[i]) {
            case reg:
                lastIsReg = true;
                codeImage[curr].rVal.ARE = ABSOLUTE;
                if(i == 0 && paramCount == 2)
                    codeImage[curr].rVal.src = isRegisterName(params[i]);
                else
                    codeImage[curr].rVal.dest = isRegisterName(params[i]);
                break;
            case local:
                codeImage[curr].lVal.ARE = RELOCATABLE;
                codeImage[curr].lVal.address = getLabel(params[i])->value;
                break;
            case value:
                codeImage[curr].imVal.ARE = ABSOLUTE;
                codeImage[curr].imVal.address = getImmedIntVal(params[i]);
                break;
            case external:
                codeImage[curr].lVal.ARE = EXTERNAL;
                codeImage[curr].lVal.address = 0;
                lbl = getLabel(params[i]);
                lbl->usages[lbl->usageCount] = curr;
                ++(lbl->usageCount);
                break;
        }
    }
    totalCode = curr + 1;
    return l;
}