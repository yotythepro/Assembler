
#include "labels.h"

label *labelTable = NULL;

label *getLabel(char *str) {
    label *p = labelTable;
    while(p) {
        if(!strcmp(str, p->name))
            return p;
        p = p->next;
    }
    return NULL;
}