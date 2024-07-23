

#ifndef UNTITLED1_LABELS_H
#define UNTITLED1_LABELS_H

#include "main.h"
#include "parsing.h"
#include <string.h>

typedef enum LabelType{dataLabel, codeLabel, externalLabel} labelType;

typedef struct Label {
    char *name; /*change to actual max of label*/
    int value;
    labelType type;
    struct Label *next;
    bool isEntry;
    int usages[256];
    int usageCount;
} label;

extern label *labelTable;

label *getLabel(char *);

#endif 
