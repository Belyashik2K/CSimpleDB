#include "../int/int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isValueNegative(const char *intString) {
    return intString[0] == '-';
}

int isValueZero(const char *intString) {
    for (int i = 0; intString[i] != '\0'; i++) {
        if (intString[i] != '0') return 0;
    }
    return 1;
}

void skipLeadingZeros(char *intString) {
    int i = 0;
    while (intString[i] == '0' && intString[i] != '\0') {
        i++;
    }

    if (i == strlen(intString)) {
        intString[0] = '0';
        intString[1] = '\0';
    }

    if (i > 0) {
        int j = 0;
        while (intString[i] != '\0') {
            intString[j] = intString[i];
            i++;
            j++;
        }
        intString[j] = '\0';
    }
}

int validateSymbols(const char *intString) {
    int minusCount = 0;
    for (int i = 0; intString[i] != '\0'; i++) {
        if (intString[i] < '0' || intString[i] > '9') return 0;
        if (intString[i] == '-') minusCount++;
        if (minusCount > 0) return 0;
    }
    return 1;
}

int isInsideIntBounds(const char *str, const int isNegative) {
    long long value = 0;
    while (*str) {
        value = value * 10 + (*str - '0');
        if ((isNegative && -value < INT_MIN) || (!isNegative && value > INT_MAX)) {
            return 0;
        }
        str++;
    }
    return 1;
}

CustomInt *intFactory(char *intString) {
    int value;
    const int isNegative = isValueNegative(intString);
    if (isNegative) {
        intString++;
    }

    if (!validateSymbols(intString)) return NULL;


    const int isZero = isValueZero(intString);
    if (!isZero) {
        skipLeadingZeros(intString);
    }

    if (!isInsideIntBounds(intString, isNegative)) return NULL;

    if (isNegative) {
        if (isZero) {
            value = 0;
        } else {
            value = -atoi(intString);
        }
    } else {
        value = atoi(intString);
    }

    CustomInt *customInt = malloc(sizeof(CustomInt));
    if (customInt == NULL) return NULL;
    customInt->value = value;
    return customInt;
}
