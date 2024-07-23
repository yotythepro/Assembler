


#include "testing.h"

void *toFree[2000];
int allocedNum = 0;

void freeAll() {
    int i;
    for(i = 0; i < allocedNum; i++)
        free(toFree[i]);
}

void printLabels() {
    label *p = labelTable;
    while(p) {
        printf("%s\t%d\t%d\n", p->name, p->value, p->type);
        p = p->next;
    }
}






