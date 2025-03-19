#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../int/int.h"
#include "../../../utils/mem_profiler/helper.h"

void freeInt(CustomInt *customInt) {
    if (!customInt) return;

    freeWrapper(customInt->field);
    freeWrapper(customInt);
}

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

char *intToString(CustomInt *self) {
    const int bufSize = strlen(self->field) + 50;
    char *buffer = (char *) mallocWrapper(bufSize);
    if (!buffer) return NULL;
    snprintf(buffer, bufSize, "%s=%d", self->field, self->value);
    return buffer;
}

int equalInt(CustomInt *self, char *other) {
    CustomInt *otherInt = intFactory(other, self->field);
    if (!otherInt) return 0;

    int result = self->value == otherInt->value;
    freeInt(otherInt);

    return result;
}

int notEqualInt(CustomInt *self, char *other) {
    return !equalInt(self, other);
}

int lessInt(CustomInt *self, char *other) {
    CustomInt *otherInt = intFactory(other, self->field);
    if (!otherInt) return 0;

    int result = self->value < otherInt->value;
    freeInt(otherInt);

    return result;
}

int greaterInt(CustomInt *self, char *other) {
    return !lessInt(self, other) && notEqualInt(self, other);
}

int lessOrEqualInt(CustomInt *self, char *other) {
    return lessInt(self, other) || equalInt(self, other);
}

int greaterOrEqualInt(CustomInt *self, char *other) {
    return greaterInt(self, other) || equalInt(self, other);
}

int compareInts(CustomInt *self, char *other, ComparisonOptionEnum option) {
    switch (option) {
        case EQUAL:
            return equalInt(self, other);
        case NOT_EQUAL:
            return notEqualInt(self, other);
        case LESS:
            return lessInt(self, other);
        case GREATER:
            return greaterInt(self, other);
        case LESS_OR_EQUAL:
            return lessOrEqualInt(self, other);
        case GREATER_OR_EQUAL:
            return greaterOrEqualInt(self, other);
        default:
            return 0;
    }
}

int updateInt(CustomInt *self, char *newValue) {
    CustomInt *newInt = intFactory(newValue, self->field);
    if (!newInt) return 0;

    self->value = newInt->value;
    freeInt(newInt);

    return 1;
}


CustomInt *intFactory(char *intString, const char *field) {
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

    CustomInt *customInt = (CustomInt *) mallocWrapper(sizeof(CustomInt));
    if (customInt == NULL) return NULL;
    customInt->value = value;

    customInt->field = strdupWrapper(field);
    if (!customInt->field) {
        freeInt(customInt);
        return NULL;
    }

    customInt->toString = intToString;
    customInt->compare = compareInts;
    customInt->update = updateInt;

    return customInt;
}
