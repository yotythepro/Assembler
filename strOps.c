

#include "strOps.h"

bool strDif(char *str1, char *str2) {
    /*the good strcmp (I hope)*/
    int i;
    i = 0;
    while(str1[i]) {
        if(str1[i] != str2[i])
            return true;
        ++i;
    }
    return (str2[i] != 0);
}